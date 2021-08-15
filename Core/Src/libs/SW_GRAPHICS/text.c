/*
 * text.c
 *
 *  Created on: 06.04.2017
 *      Author: simon
 */
#include <string.h>

#include "sw_mcu_conf.h"
#include "../SW_BOARD/sw_gpio.h"
#include "../SW_BOARD/sw_led_blink_debug.h"

#include "SW_SSD1306/framebuffer.h"
#include "SW_SSD1306/sw_ssd1306_v1.h"
#include "displays.h"
#include "text.h"

#define pgm_read_byte	*

static uint16_t _width	= SSD1306_WIDTH;
static uint16_t _height = SSD1306_HEIGHT;


/******* Ustalamy parametru bloku tekstowego ************************************************/
void graphic_set_string( T_STRING *string, wchar_t *str, int16_t x,
						int16_t y, const FONT_INFO *fontPtrPGM ) {
	string->x 			= x;
	string->y 			= y;
	string->lastPosX 	= x;
	string->lastPosY 	= y;

	string->stringLength 	= 0;
	string->str 			= str;
	string->fontPtrPGM 		= (FONT_INFO *)fontPtrPGM;
	string->onChange 		= 0;
}
/***************************************************************************************/

#ifdef PANEL_RGB
T_RGB_2ROWS colorBlack[ 8 ] = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };
/***************************************************************************************/
INLINE void draw_H_byte( volatile T_DISPLAY *buffer, int16_t x, int16_t y, uint8_t byte,
						 uint32_t color, const T_GAMMA *gamma ) {
	T_RGB_2ROWS colorTmp[ 8 ] = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };
	graphic_24bit_to_rgb( color, colorTmp, gamma );

//	if(0) {
//		if ( 0x80 & byte ) graphic_set_pixel_RAM( buffer, x + 0, y, colorTmp );
//		else			   graphic_set_pixel_RAM( buffer, x + 0, y, colorBlack );
//		if ( 0x40 & byte ) graphic_set_pixel_RAM( buffer, x + 1, y, colorTmp );
//		else			   graphic_set_pixel_RAM( buffer, x + 1, y, colorBlack );
//		if ( 0x20 & byte ) graphic_set_pixel_RAM( buffer, x + 2, y, colorTmp );
//		else			   graphic_set_pixel_RAM( buffer, x + 2, y, colorBlack );
//		if ( 0x10 & byte ) graphic_set_pixel_RAM( buffer, x + 3, y, colorTmp );
//		else			   graphic_set_pixel_RAM( buffer, x + 3, y, colorBlack );
//		if ( 0x08 & byte ) graphic_set_pixel_RAM( buffer, x + 4, y, colorTmp );
//		else			   graphic_set_pixel_RAM( buffer, x + 4, y, colorBlack );
//		if ( 0x04 & byte ) graphic_set_pixel_RAM( buffer, x + 5, y, colorTmp );
//		else			   graphic_set_pixel_RAM( buffer, x + 5, y, colorBlack );
//		if ( 0x02 & byte ) graphic_set_pixel_RAM( buffer, x + 6, y, colorTmp );
//		else			   graphic_set_pixel_RAM( buffer, x + 6, y, colorBlack );
//		if ( 0x01 & byte ) graphic_set_pixel_RAM( buffer, x + 7, y, colorTmp );
//		else			   graphic_set_pixel_RAM( buffer, x + 7, y, colorBlack );
//	} else {
		if ( 0x80 & byte ) graphic_set_pixel_RAM1( buffer, x + 0, y, colorTmp );
		if ( 0x40 & byte ) graphic_set_pixel_RAM1( buffer, x + 1, y, colorTmp );
		if ( 0x20 & byte ) graphic_set_pixel_RAM1( buffer, x + 2, y, colorTmp );
		if ( 0x10 & byte ) graphic_set_pixel_RAM1( buffer, x + 3, y, colorTmp );
		if ( 0x08 & byte ) graphic_set_pixel_RAM1( buffer, x + 4, y, colorTmp );
		if ( 0x04 & byte ) graphic_set_pixel_RAM1( buffer, x + 5, y, colorTmp );
		if ( 0x02 & byte ) graphic_set_pixel_RAM1( buffer, x + 6, y, colorTmp );
		if ( 0x01 & byte ) graphic_set_pixel_RAM1( buffer, x + 7, y, colorTmp );
//	}
}

/*********************************************** VERSJA2 *************************************************/
INLINE uint8_t graphic_draw_char_RAM( volatile T_DISPLAY *buffer, int16_t x, int16_t y, wchar_t charCode, uint32_t  color,
									  uint32_t bg, uint8_t drawLine, FONT_INFO *fontPtrPGM, const T_GAMMA *gamma ) {
	uint8_t		charWidth;
	uint16_t	charOffset;

	uint8_t		charHeight			= fontPtrPGM->heightPixels;
	uint8_t		charAddressStart	= fontPtrPGM->startChar;
	uint8_t		transCode 			= polish_letters(UNICODE, charCode);

	// Rysujemy SPACJĘ i wychodzimy -------------------------------------------------------------------
	if (charCode == L' ') {
		graphic_fill_rectangle_RAM( buffer, x, y, fontPtrPGM->spacePixels, charHeight, bg, gamma );
		return fontPtrPGM->spacePixels;
	}
	//--------------------------------------------------------------------------------------------------
	if (transCode == ASCII) {
		// Czytamy ile bitów zajmuje odległość czcionki
		charWidth  = fontPtrPGM->charInfo[ charCode - charAddressStart ].widthBits;
		// Czytamy ile bitów zajmuje odległość między znakami
		charOffset = fontPtrPGM->charInfo[ charCode - charAddressStart ].offset;
	} else {
		charWidth  = fontPtrPGM->charInfo[ fontPtrPGM->sizeArray - (PL_CHARACTERS_COUNT - transCode) ].widthBits;
		charOffset = fontPtrPGM->charInfo[ fontPtrPGM->sizeArray - (PL_CHARACTERS_COUNT - transCode) ].offset;
	}

	for ( uint8_t iY = 0; iY < charHeight; iY++ ) {					// Kolejne wiersze bitmapy czcionki
		for ( uint8_t jX = 0; jX < (charWidth - 1)/8 + 1; jX++ ) {	// Kolejne kolumny
			// Pobieramy wiersz czcionki
			uint8_t byte = fontPtrPGM->data[ charOffset + jX + iY*( ((uint8_t)(charWidth - 1)/8) + 1) ];
			if (byte != 0)
				draw_H_byte( buffer, x + jX*8, y + iY, byte, color, gamma );
		}
	}// Koniec pętli do wierszy
	return charWidth;
}
/*********************************************************************************************************/
#elif defined COLOR_MONO
uint8_t graphic_draw_char_RAM( volatile T_DISPLAY *buffer, int16_t x, int16_t y, wchar_t charCode, uint32_t  color,
									  uint32_t bg, uint8_t drawLine, FONT_INFO *fontPtrPGM, const T_GAMMA *gamma ) {
	uint8_t		charWidth;
	uint16_t	charOffset;
	uint8_t		charHeight			= fontPtrPGM->heightPixels;
	uint8_t		charAddressStart	= fontPtrPGM->startChar;
	uint8_t		transCode 			= polish_letters(UNICODE, charCode);
	uint8_t 	size 				= 1;
	// Rysujemy SPACJĘ i wychodzimy -------------------------------------------------------------------
	if (charCode == L' ') {
		graphic_fill_rectangle_RAM( buffer, x, y, fontPtrPGM->spacePixels, charHeight, bg, gamma );
		return fontPtrPGM->spacePixels;
	}
	//--------------------------------------------------------------------------------------------------
	if (transCode == ASCII) {
		// Czytamy ile bitów zajmuje odległość czcionki
		charWidth  = fontPtrPGM->charInfo[ charCode - charAddressStart ].widthBits;
		// Czytamy ile bitów zajmuje odległość między znakami
		charOffset = fontPtrPGM->charInfo[ charCode - charAddressStart ].offset;
	} else {
		charWidth  = fontPtrPGM->charInfo[ fontPtrPGM->sizeArray - (PL_CHARACTERS_COUNT - transCode) ].widthBits;
		charOffset = fontPtrPGM->charInfo[ fontPtrPGM->sizeArray - (PL_CHARACTERS_COUNT - transCode) ].offset;
	}

	if((x >= _width)					||	// Clip right
		(y >= _height)					||	// Clip bottom
		((x + charWidth  * size - 1) < 0)||	// Clip left
		((y + charHeight * size - 1) < 0))	// Clip top
	return 0;

	if (charCode == L' ') {
		graphic_fill_rectangle_RAM( buffer, x , y, charWidth*size, charHeight*size, bg, NULL );

		return fontPtrPGM->spacePixels;
	}
	int8_t i, k, j=0;
	uint8_t calkow = (charWidth - 1)/8; // Sprawdzamy w ilu bitach mamy szerokość czcionki
	uint8_t line;

	for ( i=0; i < charHeight; i++ ) // Kolejne wiersze
	{
		for ( j = 0; j < calkow+1; j++ ) {  // Kolejne kolumny

			line = pgm_read_byte( fontPtrPGM->data + charOffset + (calkow + 1)*i + j );

			for ( k=0; k<8; k++ ) {
				if ( (line & (1 << (7-k))) && ( (k + 8*j) < charWidth) ) {    // Ekstrahujemy kolejne bity
						graphic_set_pixel_RAM( buffer, x + 8*j + k, y+i, (uint8_t *)&color );
				}
				else if( (bg!=color) && ( (k + 8*j) < charWidth) ) {
						graphic_set_pixel_RAM( buffer, x + 8*j + k, y+i, (uint8_t *)&bg );
				}
			}
		} // Koniec rysowania
	}// Koniec pętli do wierszy

	return charWidth;
}

#endif
/***************************************************************************************/


/******************************** Draw the string in RAM (buffer)	 *********************************/
uint16_t graphic_puts_RAM( volatile T_DISPLAY *buffer, T_STRING *string, uint8_t textSize, uint32_t color, uint32_t bg, const T_GAMMA *gamma ) {
	int16_t cursorX = string->x;
	int16_t cursorY = string->y;
	uint8_t charWidth;

	textSize = FONTx1;	// To być może będzie niepotrzebne

	if ( string->onChange ) {
		graphic_fill_rectangle_RAM( buffer, string->lastPosX, string->lastPosY, string->stringLength,
									string->fontPtrPGM->heightPixels, bg, gamma );
		string->onChange = 0;
	}
	string->lastPosX = string->x;
	string->lastPosY = string->y;

	wchar_t * str = string->str;

	while ( *str ) {
		charWidth = graphic_draw_char_RAM( buffer, cursorX, cursorY, *str++, color, bg, textSize, string->fontPtrPGM, gamma );
		cursorX += textSize * (charWidth + string->fontPtrPGM->interspacePixels );
	}
	return  string->stringLength = cursorX - string->x; //Zwraca długość napisu w pikselach
}
/*********************************************************************************************************/

/*********************************************************************************************************/
void graphic_clear_last_text( volatile T_DISPLAY *buffer, T_STRING *string, uint32_t bg, const T_GAMMA *gamma ) {
	graphic_fill_rectangle_RAM( buffer, string->x, string->y, string->stringLength,
								string->fontPtrPGM->heightPixels, bg, gamma );
}
/*********************************************************************************************************/

/******************* Duplikuje tablice typu char do wideChar *******************************/
uint8_t graphic_char_to_wide( wchar_t* dest, char* source ) {
	int i = 0;
	char * ptr = source;

    while(ptr[i]) {
        dest[i] = (char)source[i];
        i++;
    }
    dest[i] = '\0';
	return i; // Zwraca długość string'a bez znaku kontrolnego '\0'
}
/*******************************************************************************************/

/**************** Specjalna funkcja rysująca 1 znak ****************************************/
void graphic_spec_char_RAM( volatile T_DISPLAY *buffer, T_STRING *string, int data, uint8_t charLine, uint32_t color,
							uint32_t bg, const T_GAMMA *gamma ) {
	if ( string->onChange ) {
		graphic_fill_rectangle_RAM( buffer, string->x, string->y, 4,
									string->fontPtrPGM->heightPixels, bg, gamma);
		string->onChange = 0;
	}
	char	bufer [4];
	wchar_t bufer2[4];

	(void)graphic_char_to_wide( bufer2, itoa( data, bufer, 10 ) );
	string->str = bufer2;
	string->stringLength = graphic_draw_char_RAM( buffer, string->x, string->y, *bufer2, color, bg,
												  charLine, string->fontPtrPGM, gamma );
}
/*******************************************************************************************/

/********************* Draw the numbers on screen ******************************************/
uint16_t graphic_puts_int_RAM( volatile T_DISPLAY *buffer, T_STRING *string, int data, uint8_t textSize,
						   	   uint32_t color, uint32_t bg, const T_GAMMA *gamma ) {
	char bufer[16];
	wchar_t bufer2[16];
	string->str = bufer2;

	itoa( data, bufer, 10 );

	(void)graphic_char_to_wide( bufer2, bufer );

	return string->stringLength = graphic_puts_RAM( buffer, string, textSize, color, bg, gamma );
}

/*******************************************************************************************/

/********************* Wystawiam liczby na ekran, zwracam długość stringa w pikselach ******/
void graphic_puts_hex_RAM( volatile T_DISPLAY *buffer, T_STRING *string, uint16_t data, uint8_t textSize,
						   uint32_t color, uint32_t bg, const T_GAMMA *gamma ) {
	char buf [10];
	char buf2[10];

	itoa(data, buf, 16);

	if (data <= 0x000f) {
		strcpy(buf2, "0x000");
	} else
	if ( (data>0x000f)&&(data<=0x00ff) ) {
		strcpy(buf2, "0x00");
	} else
	if ( (data>0x00ff)&&(data<=0x0fff) ) {
		strcpy(buf2, "0x0");
	} else
	if ( data>0x0fff ) {
		strcpy(buf2, "0x");
	}
	strcat(buf2, buf);

	(void)graphic_char_to_wide( string->str, buf2 );
	string->stringLength = graphic_puts_RAM( buffer, string, textSize, color, bg, gamma );
}
/********************************************************************************************/

/********************* Draw the numbers on screen ******************************************/
void rtc_puts_int_RAM( volatile T_DISPLAY *buffer, T_STRING *string, int data, uint8_t textSize,
					   uint32_t color, uint32_t bg, const T_GAMMA *gamma ) {
	char	buffer3[16] = "0";
	char 	bufer1[16];
	wchar_t buffer2[16];

	char *str = bufer1;
	string->str = buffer2;

	itoa( data, bufer1, 10 );

	if ( data > -1 && data < 10) {
		strcat( buffer3, bufer1 );
		str = buffer3;
	}
	(void)graphic_char_to_wide( buffer2, str );
	string->stringLength = graphic_puts_RAM( buffer, string, textSize, color, bg, gamma );
}

#define FLOAT_PRECISION	6		// Numbers to display + dot and sign character
static void graphic_puts_float_RAM( volatile T_DISPLAY *buffer, T_STRING *string, float data, uint8_t textSize,
							 	 	 uint32_t color, uint32_t bg, const T_GAMMA *gamma ) {
	char 	text_char [ FLOAT_PRECISION + 3 ];	// Sign, dot and '\0'
	wchar_t	text_wchar[ FLOAT_PRECISION + 3 ];
	char *ptr_char 	= text_char;
	string->str 	= text_wchar;
	uint32_t dec;

	if ( data < 0 ) {
		*text_char = '-';
		data = data * -1;					// Only number module
	} else {
		*text_char = '+';					// Space for sign character
	}
	dec 		= (uint32_t)data;			// Integer part of a number;
	uint8_t i 	= 1;						// First character is sign
	itoa( dec, text_char + i , 10 );

	while ( *ptr_char ) {					// Find number of integer digits
		i++;
		ptr_char++;
	} i--;
	text_char[ i++ ] = '.';					// Place '.' character';
	data = data - (float32_t)dec;			// Fractional part of a number

	do {									// Find first zeros after the dot
		data = (float32_t)data*10;
		dec  = (uint32_t)data;
		if ( dec == 0 ) {
			text_char[ i++ ] = '0';
		}
	} while ( (dec == 0) && (i <= FLOAT_PRECISION ) );

	dec = (uint32_t)powf( 10, FLOAT_PRECISION - (i - 1) )*data;   // 10000000 * data
	itoa( (uint32_t)dec, text_char + i, 10 );

	(void)graphic_char_to_wide( text_wchar, text_char );
    string->stringLength = graphic_puts_RAM( buffer, string, textSize, color, bg, gamma );
}


void TEXT_display_number( int16_t x, int16_t y, int16_t number, T_STRING * Text ) {
	wchar_t String[10];
	Text->str = String;
	Text->x = x;
	Text->y = y;
	Text->onChange = 1;
	Text->fontPtrPGM = (FONT_INFO *)&MicrosoftSansSerif8ptFontInfo_var;
	graphic_puts_int_RAM( TextBuffer, Text, number, FONTx1, (uint32_t)WhiteColor, (uint32_t)BlackColor, &GammaRGB );
}
void TEXT_display_float( int16_t x, int16_t y, float number, T_STRING * Text ) {
	wchar_t String[10];
	Text->str = String;
	Text->x = x;
	Text->y = y;
	Text->onChange = 1;
	Text->fontPtrPGM = (FONT_INFO *)&MicrosoftSansSerif8ptFontInfo_var;
	graphic_puts_float_RAM( TextBuffer, Text, number, FONTx1, (uint32_t)WhiteColor, (uint32_t)BlackColor, &GammaRGB );
}
void TEXT_display_string( int16_t x, int16_t y, wchar_t * str, T_STRING * Text ) {
	wchar_t String[10];
	Text->str = String;
	Text->x = x;
	Text->y = y;
	Text->str = str;
	Text->fontPtrPGM = (FONT_INFO *)&MicrosoftSansSerif8ptFontInfo_var ;
	graphic_puts_RAM( TextBuffer, Text, FONTx1, (uint32_t)WhiteColor, (uint32_t)BlackColor, &GammaRGB );
}




static T_STRING Text1;
static wchar_t text1[] = L"";

static T_STRING Text2;
static wchar_t text2[] = L"";

/********************************************************************************************/
void test_text_param( FONT_INFO * font ) {
	graphic_set_string( &Text1, text1, 0, 0, font );
	graphic_set_string( &Text2, text2, 0, 32, font );
}
/********************************************************************************************/
const wchar_t string[] = L"Simon  idę spaś";
//const wchar_t string[] = L"TVP1: Indiana Jones to najlepszy film w historii";

/********************************************************************************************/
void test_text_display_number( uint8_t number ) {
	Text1.str = (wchar_t *)string;
	graphic_puts_RAM( 	  TextBuffer, &Text1, FONTx1, 		  (uint32_t)WhiteColor, (uint32_t)BlackColor, &GammaRGB );
	graphic_puts_int_RAM( TextBuffer, &Text2, number, FONTx1, (uint32_t)WhiteColor, (uint32_t)BlackColor, &GammaRGB );
}
/********************************************************************************************/

/********************************************************************************************/
void text_floating_string( volatile T_DISPLAY * buffer ) {
	static int16_t ix = 0, iy = 0;
	static int8_t dirX = 1, dirY = 1;
	uint8_t a = Text1.fontPtrPGM->heightPixels;
	static uint8_t b;

	b = 10; a = 10;

	if ( dirX == 1 ) ix++;				// Zmiana kierunku
	else ix--;

	if ( dirY == 1 ) iy++;				// Zmiana kierunku
	else iy--;

	if ( ix == SCREEN_WIDTH - b ) dirX = -1;			// Zmiana kierunku
	if ( ix == 0) dirX = 1;

	if ( iy == SCREEN_HEIGHT - a ) dirY = -1;			// Zmiana kierunku
	if ( iy == 0) dirY = 1;

	Text1.str = (wchar_t *)string;
	Text1.x = ix;
	Text1.y = iy;

	graphic_fill_rectangle_RAM( buffer, ix, 0, a, b, WhiteColor, &GammaRGB );

//	b = graphic_puts_RAM( buffer, &Text1, FONTx1, (uint32_t)MagentaColor, (uint32_t)BlackColor, &GammaRGB );
}
/********************************************************************************************/

#define	DIR_RIGHT		(int8_t)1
#define	DIR_LEFT		(int8_t)-1

/********************************************************************************************/
void text_scrolling_text( volatile T_DISPLAY * buffer, uint32_t color, int8_t dir ) {
	static int16_t ix = 0, iy = 0;
	static uint16_t b = 0;
//	if (b==0) {
		b = graphic_puts_RAM( buffer, &Text1, FONTx1, color, (uint32_t)BlackColor, &GammaRGB );
//	}
	Text1.str = (wchar_t *)string;

//	if ( b > SCREEN_WIDTH && dir != 0) {
		if (dir == DIR_RIGHT ) Text1.x = ++ix;
		else if(dir==DIR_LEFT) Text1.x = --ix;
//	} else {
//		Text1.x = SCREEN_WIDTH/2 - b/2;
//	}
	Text1.y = iy;

	if ( ix == SCREEN_WIDTH && dir == DIR_RIGHT ) {
		ix = -b;
	} else
	if ( ix == -b && dir == DIR_LEFT ) {
		ix = SCREEN_WIDTH - 1;
	}
}
/********************************************************************************************/

void display_integer( volatile T_DISPLAY * buffer, uint32_t color, int data ) {
	clear_buffer_RAM( buffer );
	graphic_puts_int_RAM( buffer, &Text2, data, FONTx1, color, BlackColor, &GammaRGB );
}



