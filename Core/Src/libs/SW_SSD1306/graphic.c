/*
 * graphic.c
 *
 *  Created on: 28 paź 2015
 *      Author: simon
 */

#ifdef __AVR__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#else
#include <stdint.h>
#include "../SW_TIMERS/sw_soft_timers.h"
#define _delay_ms	delay_ms
#define pgm_read_byte	*
#define pgm_read_word	*
#endif

#include <string.h>


#include "sw_ssd1306.h"

#include "graphic.h"
#include "bitmaps.h"

#include "fonts_pl.h"

/********************** ******************************************************/
#define swap(a, b) { int16_t t = a; a = b; b = t; }

/******* Ustalamy aktualne parametry czcionki, wypełniamy strukturę w pamięci RAM ***********/

void graphic_set_current_font (FONT_INFO * font, FONT_INFO *currentFont) {
	currentFont->heightPixels		= pgm_read_byte(&font->heightPixels);
	currentFont->startChar			= pgm_read_byte(&font->startChar);
	currentFont->interspacePixels	= pgm_read_byte(&font->interspacePixels);
	currentFont->spacePixels		= pgm_read_byte(&font->spacePixels);
	currentFont->charInfo 			= (FONT_CHAR_INFO*)pgm_read_word(&font->charInfo);
	currentFont->data				= (char*)pgm_read_word(&font->data);
//	currentFont->FontFileName		= (char*)pgm_read_word(&font->FontFileName);
	currentFont->sizeArray			= pgm_read_word(&font->sizeArray);
}
/********************************************************************************************/

/*********************** Rysuje w RAM (buforze) pojedynczy pixel ****************************/
void graphic_draw_pixel_RAM( int16_t x, int16_t y, uint8_t bw ) {
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
    return;
  if (bw)
	  mem_128x64_buf[ x + (y/8)*_width ] |=  (1<<(y%8)); // zapal pixel
  else
	  mem_128x64_buf[ x + (y/8)*_width ] &= ~(1<<(y%8)); // zgaś pixel
}
/********************************************************************************************/

/******************** Rysuje w RAM (buforze) linię, za pomocą set/clear pixel ***************/
void graphic_draw_line_RAM( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color ) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }
  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
    	graphic_draw_pixel_RAM(y0, x0, color);
    } else {
    	graphic_draw_pixel_RAM(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}
/********************************************************************************************/

/************************ Rysuje w buforze bitmapę z pamięci FLASH **************************/
void graphic_draw_bitmap_PGM( int16_t x, int16_t y, const uint8_t *bitmap,
							  int16_t w, int16_t h, uint16_t color ) {
  int16_t i, j, byteWidth = (w + 7) / 8;
  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
    	  graphic_draw_pixel_RAM(x+i, y+j, color);
      }
    }
  }
}
/********************************************************************************************/

void graphic_draw_fast_Vline_RAM(int16_t x, int16_t y, int16_t h, uint8_t color) {
	graphic_draw_line_RAM(x, y, x, y+h-1, color);
}

void graphic_draw_fast_Hline_RAM(int16_t x, int16_t y, int16_t w, uint8_t color) {
	graphic_draw_line_RAM(x, y, x+w-1, y, color);
}

void graphic_fill_rect_RAM(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
  for (int16_t i=x; i<x+w; i++) {
	  graphic_draw_fast_Vline_RAM(i, y, h, color);
  }
}
/********************************************************************************************/

/******************************** Wypałnia kolorem prostokąt *********************************/
void graphic_fill_screen_RAM(uint8_t color) {
	graphic_fill_rect_RAM(0, 0, _width, _height, color);
}
/*********************************************************************************************/

/********************** Rysujemy pojedynczy znak w pamięci RAM (buforze) *********************************/
uint8_t graphic_draw_char_RAM(int16_t x, int16_t y, wchar_t charCode, uint8_t color, uint16_t bg,
					  uint8_t size, FONT_INFO fontPtrPGM) {
	uint16_t	charOffset;
	uint8_t		charWidth;
	uint8_t		charHeight			= fontPtrPGM.heightPixels;
	uint8_t		charAddressStart	= fontPtrPGM.startChar;
	uint8_t		transCode 			= polish_letters(UNICODE, charCode);

	if (transCode == ASCII) {
		charWidth  = pgm_read_byte( &(fontPtrPGM.charInfo[ charCode - charAddressStart].widthBits) );
		charOffset = pgm_read_word( &(fontPtrPGM.charInfo[ charCode - charAddressStart].offset) );
	}
	else {
		charWidth  = pgm_read_byte( &(fontPtrPGM.charInfo[ fontPtrPGM.sizeArray -(PL_CHARACTERS_COUNT - transCode) ].widthBits) );
		charOffset = pgm_read_word( &(fontPtrPGM.charInfo[ fontPtrPGM.sizeArray -(PL_CHARACTERS_COUNT - transCode) ].offset)    );
	}
	if((x >= _width)					||	// Clip right
		(y >= _height)					||	// Clip bottom
		((x + charWidth  * size - 1) < 0)||	// Clip left
		((y + charHeight * size - 1) < 0))	// Clip top
	return 0;

	if (charCode == L' ') {
		graphic_fill_rect_RAM( x , y, charWidth*size, charHeight*size, bg);
		return fontPtrPGM.spacePixels;
	}
	int8_t i, k, j=0;
	uint8_t calkow = (charWidth - 1)/8; // Sprawdzamy w ilu bitach mamy szerokość czcionki
	uint8_t line;

	for (i=0; i < charHeight; i++ ) // Kolejne wiersze
	{
		for ( j = 0; j < calkow+1; j++) {  // Kolejne kolumny

			line = pgm_read_byte( fontPtrPGM.data + charOffset + (calkow + 1)*i + j );

			for (k=0; k<8; k++) {
				if ( (line & (1 << (7-k))) && ( (k + 8*j) < charWidth) ) {    // Ekstrahujemy kolejne bity
					if (size==1) // default size
						graphic_draw_pixel_RAM( x + 8*j + k, y+i, color);
					else		 // big size
						graphic_fill_rect_RAM( x + (8*j +k)*size , y+(i*size), size, size, color);
				}
				else if( (bg!=color) && ( (k + 8*j) < charWidth) ) {
					if (size==1) // default size
						graphic_draw_pixel_RAM( x + 8*j + k, y+i, bg);
					else 		 // big size
						graphic_fill_rect_RAM( x + (8*j +k)*size , y+(i*size), size, size, bg);
				}
			}
		} // Koniec rysowania
	}// Koniec pętli do wierszy
return charWidth;
}
/*********************************************************************************************************/

/******************************** Rysujemy napis w pamięci RAM (buforze), z RAM **************************/
uint8_t graphic_puts_RAM(int x, int y, wchar_t *str, uint8_t textSize, uint8_t color,
				 uint8_t bg, FONT_INFO fontPtrPGM) {
	int cursorX = x;
	int cursorY = y;
	uint8_t charWidth;

	while (*str) {
		charWidth = graphic_draw_char_RAM(cursorX, cursorY, *str++, color, bg, textSize, fontPtrPGM);
		cursorX += textSize * (charWidth + fontPtrPGM.interspacePixels);
	}
	return cursorX - x; //Zwraca długość napisu w pikselach
}
/*********************************************************************************************************/

/******************************* Rysujemy napis w pamięci RAM (buforze), z FLASH *************************/
uint8_t graphic_puts_RAM_P(int x, int y, const wchar_t *strPGM, uint8_t textSize, uint8_t color,
				   uint8_t bg, FONT_INFO fontPtrPGM) {
	int cursorX = x;
	int cursorY = y;
	uint8_t txtWidth;

	while (pgm_read_word(strPGM)) {
		wchar_t znak = pgm_read_word(strPGM++);
		txtWidth = graphic_draw_char_RAM(cursorX, cursorY, znak , color, bg, textSize, fontPtrPGM);
		cursorX += textSize * (txtWidth + fontPtrPGM.interspacePixels);
	}
	return cursorX - x; //Zwraca długość napisu w pikselach
}
/*********************************************************************************************************/

/******************* Duplikuje tablice typu char do wideChar *******************************/
uint8_t graphic_char_to_wide(wchar_t* dest, char* source)
{
	int i = 0;
	char * ptr = source;

    while(ptr[i]) {
        dest[i] = (char)source[i];
        i++;
    }
    dest[i] = '\0';
	return i; // Zwraca długość stringa bez znaku kontrolnego '\0'
}
/*******************************************************************************************/

/********************* Wystawiam liczby na ekran, zwracam długość stringa w pikselach ******/
uint8_t graphic_puts_int_RAM(int x, int y, int data, uint8_t textSize,
					 uint8_t color,uint8_t bg, FONT_INFO charPtrPGM) {
	char bufer[16];
	wchar_t bufer2[16];
	(void)graphic_char_to_wide( bufer2, itoa(data, bufer, 10) );

	return graphic_puts_RAM(x, y, bufer2, textSize, color, bg, charPtrPGM);
}
/*******************************************************************************************/

uint8_t graphic_puts_hex_RAM(int x, int y, uint16_t data, uint8_t textSize,
					 uint8_t color, uint8_t bg, FONT_INFO fontPtrPGM) {
	wchar_t buf2[6];
	uint8_t len;

	if (data <= 0xf) {
		(void)graphic_char_to_wide(buf2, "0x000");
	}
	else if ( (data>0xf)&&(data<=0xff) ) {
		(void)graphic_char_to_wide(buf2, "0x00");
	}
	else if ( (data>0xff)&&(data<=0xfff) ) {
		(void)graphic_char_to_wide(buf2, "0x0");
	}
	else if ( data>0xfff ) {
		(void)graphic_char_to_wide(buf2, "0x");
	}

	len = graphic_puts_RAM(x, y, buf2, textSize, color, bg, fontPtrPGM);

	char buf[16];
	graphic_char_to_wide( buf2, itoa(data, buf, 16) );

	return len + graphic_puts_RAM(x + len*textSize, y, buf2, textSize, color, bg, fontPtrPGM);
}
/********************************************************************************/

