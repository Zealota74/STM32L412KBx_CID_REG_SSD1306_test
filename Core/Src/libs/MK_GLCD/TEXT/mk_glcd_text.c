/*                  e-gadget.header
 * mk_glcd_text.c
 *
 *  Created on: 2019-05-21
 *    Modyfied: 2019-06-19 09:28:21
 *      Author: Miros�aw Karda�
 *         web: www.atnel.pl
 *
 *	Library: MK GLCD  ver: 1.1a
 *
 * 	 Obs�ugiwane wy�wietlacze/chipsety/magistrale:
 * 
 * 	 1. OLED 128x64 - SSD1306 I2C/SPI
 * 	 2. OLED 128x32 - SSD1306 I2C/SPI
 * 	 3. COG 128x64  - ST7565R SPI
 * 	 4. NOKIA 84x48 - PCD8544 SPI
 * 	 5. COG 128x64  - UC1701x SPI
 * 
 * 	 Biblioteka "MK GLCD" wymaga do prawid�owej pracy
 * 	 jednej z dw�ch bibliotek:
 * 
 * 	 1. MK_I2C		https://sklep.atnel.pl/pl/p/0581_0582-MK-I2C-AVR-Biblioteka-C/244
 * 	 2. MK_SPI		https://sklep.atnel.pl/pl/p/0567_0570-MK-SPI-AVR-Biblioteka-C-/241
 * 
 * 	 F_CPU: 1MHz - 24MHz
 * 	 MCU: all ATmega uC with minimum 2Kb RAM
 * 	
 * 	 Uwaga! przed kompilacj� nale�y doda� opcj� w ustawieniach toolchaina
 * 	 C/C++ Build/Settings/AVR C Linker/General/Other Arguments
 * 	 -Wl,-gc-sections
 * 	
 *
 *
 */
#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#elif defined STM32F3 || defined STM32L4
#include "sw_mcu_conf.h"
#include "../../SW_BOARD/sw_gpio.h"
#include "../../SW_TIMERS/sw_soft_timers.h"
#include "../../sw_ltoa.h"
#define PROGMEM
#define pgm_read_byte	*
#define pgm_read_word	*
#define _delay_ms		delay_ms
#endif

#include <string.h>
#include <stdlib.h>

#include "../mk_glcd_base.h"
#include  "mk_glcd_text.h"

#include "../FONTS/mk_fonts.h"

uint8_t text_align;

FONT_INFO currentFont;

void setCurrentFont( const FONT_INFO * font ) {
	currentFont.down_space			= pgm_read_byte(&font->down_space);
	currentFont.heightPixels 		= pgm_read_byte(&font->heightPixels);
	currentFont.startChar 			= pgm_read_byte(&font->startChar);
	currentFont.interspacePixels	= pgm_read_byte(&font->interspacePixels);
	currentFont.spacePixels 		= pgm_read_byte(&font->spacePixels);
	currentFont.charInfo 			= (FONT_CHAR_INFO*)pgm_read_word(&font->charInfo);
	currentFont.data 				= (uint8_t*)pgm_read_word(&font->data);
	currentFont.FontFileName		= (char*)pgm_read_word(&font->FontFileName);
#if USE_TWO_FONTS_BITORIENT == 1
	currentFont.bitOrientation		= pgm_read_byte(&font->bitOrientation);
#endif
}








static void send_glyph_byte( int x, int y, uint8_t bajt, uint8_t height, uint8_t mode ) {

	for( uint8_t i=0; i<height; i++ ) {
		if( bajt & 0x01 ) glcd_setPixel( x,y, mode );
//		else {
//			set_background_pixel( x, y, mode );
//		}
		bajt>>=1;
		y++;
	}
}


#if USE_TWO_FONTS_BITORIENT == 1
static void send_glyph_byteB( int x, int y, uint8_t bajt, uint8_t width, uint8_t mode ) {

	for( uint8_t i=0; i<width; i++ ) {
		if( bajt & 0x80 ) glcd_setPixel( x,y, mode );
//		else {
//			set_background_pixel( x, y, mode );
//		}
		bajt<<=1;
		x++;
	}
}


static void send_font_bitmapB( int x, int y, const uint8_t *glyph, int glyphHeight, int glyphWidth, uint8_t mode ) {

	uint8_t i, j, k, bajt;
	uint16_t p=0;


	for( i=0; i<glyphHeight; i++ ) {
		for( j=0, k=0; j<glyphWidth; j+=8, k++ ) {
			bajt = pgm_read_byte( &glyph[ p++ ] );
			if( ((k+1)*8)<=glyphWidth ) send_glyph_byteB( x+(k*8), y, bajt, 8, mode );
			else send_glyph_byteB( x+(k*8), y, bajt, glyphWidth-(k*8), mode );
		}
		y++;
	}

}
#endif


static void send_font_bitmap( int x, int y, const uint8_t *glyph, int glyphHeight, int glyphWidth, uint8_t mode ) {

	uint8_t i, j, k, bajt;
	uint16_t p=0;

	for( i=0; i<glyphWidth; i++ ) {
		for( j=0, k=0; j<glyphHeight; j+=8, k++ ) {
			bajt = pgm_read_byte( &glyph[ p++ ] );
			if( ((k+1)*8)<=glyphHeight ) send_glyph_byte( x, y+(k*8), bajt, 8, mode );
			else send_glyph_byte( x, y+(k*8), bajt, glyphHeight-(k*8), mode );
		}
		x++;
	}

}



void glcd_put_char( int x, int y, char c, uint8_t mode ) {

	uint8_t gH, gW, gS, gIS;//,i;
	uint16_t offset;
	uint8_t startChar = currentFont.startChar;
	uint8_t * glyph = (uint8_t*)currentFont.data;

	gH = currentFont.heightPixels;
	gIS = currentFont.interspacePixels;
	gS = currentFont.spacePixels;


#if USE_TWO_FONTS_BITORIENT == 1
	void (*fun)( int x, int y, const uint8_t *glyph, int glyphHeight, int glyphWidth, uint8_t mode );

	if( currentFont.bitOrientation == 0 ) fun = send_font_bitmap;
	else fun = send_font_bitmapB;

#endif


	if( c && c != ' ' ) {

		gW = pgm_read_byte( &currentFont.charInfo[ c - startChar  ].widthBits );
		offset = pgm_read_word( &currentFont.charInfo[ c - startChar  ].offset);
		if( (x + gW - 1 ) >= 0 && x<GLCD_WIDTH ) {
#if USE_TWO_FONTS_BITORIENT == 1
			fun(x, y, glyph+offset, gH, gW, mode );
#else
			send_font_bitmap(x, y, glyph+offset, gH, gW, mode );
#endif
		}
		x = x + gW + gIS;
	} else {
//		// rysowanie spacji
//		for(offset=0;offset<gS;offset++) {
//			for(uint8_t i=0;i<gH;i++) {
//				glcd_setPixel( x+offset, y+i, 0 );
//			}
//		}
		x += gS + gIS;
	}
	cur_x = x;
	cur_y = y;
}


void glcd_put_char1( char c, uint8_t mode ) {
	glcd_put_char( cur_x, cur_y, c, mode );
}


void glcd_puts( int x, int y, char * s, uint8_t mode ) {

	if( _center == text_align ) x = x/2 - text_len(s)/2 + currentFont.interspacePixels/2 + 1;
	else if( _right == text_align ) x = x - text_len(s) + currentFont.interspacePixels + 1;

	cur_x = x;
	cur_y = y;

	if( mode<2 ) glcd_fillRect(x,y, text_len(s), currentFont.heightPixels, mode^1 );

	while( *s ) glcd_put_char( cur_x, cur_y, *s++, mode );
}

void glcd_puts1( char * s, uint8_t mode ) {

	glcd_puts( cur_x, cur_y, s, mode );
}

void glcd_puts_P( int x, int y, const char * s, uint8_t mode ) {

	if( _center == text_align ) x = x/2 - text_len_P(s)/2 + currentFont.interspacePixels/2 + 1;
	else if( _right == text_align ) x = x - text_len_P(s) + currentFont.interspacePixels + 1;

	cur_x = x;
	cur_y = y;

	if( mode<2 ) {
		glcd_fillRect(x,y, text_len_P(s), currentFont.heightPixels, mode^1 );
	}

	char c;
	while( ( c = pgm_read_byte(s++) ) ) glcd_put_char( cur_x, cur_y, c, mode );
}

void glcd_puts1_P( const char * s, uint8_t mode ) {
	glcd_puts_P( cur_x, cur_y, s, mode );
}


void glcd_int( int x, int y, int32_t val, uint8_t mode ) {
	char buf[17];
	ltoa(val, buf, 10 );

	cur_x = x;
	cur_y = y;
	glcd_puts( x, y, buf, mode );
}

void glcd_int1( int32_t val, uint8_t mode ) {
	glcd_int( cur_x, cur_y, val, mode );
}

void glcd_uint( int x, int y, uint32_t val, uint8_t mode ) {
	char buf[17];
	ultoa(val, buf, 10 );
	glcd_puts( x, y, buf, mode );
}

void glcd_uint1( uint32_t val, uint8_t mode ) {
	glcd_uint( cur_x, cur_y, val, mode );
}


void glcd_hex( int x, int y, uint32_t val, uint8_t mode ) {
	char buf[5];
	ultoa(val, buf, 16 );
	glcd_puts( x, y, buf, mode );
}

void glcd_hex1( uint32_t val, uint8_t mode ) {
	glcd_hex( cur_x, cur_y, val, mode );
}


// konwersja do postaci binarnej liczb max 32-bitowych
// ARG:
// val - liczba do konwersji
// len - ilo�� znak�w postaci binarnej z zerami nieznacz�cymi
void glcd_bin( int x, int y, uint32_t val, uint8_t len, uint8_t mode ) {
	char str[len+1];
	memset( str, 0, len+1 );
	for( int8_t i=0, k=len-1; i<len; i++ ) {
		uint32_t a = val >> k;
		if( a & 0x0001 ) str[k]='1'; else str[k]='0';
		k--;
	}
	strrev( str );
	glcd_puts( x, y, str, mode );
}

void glcd_bin1( uint32_t val, uint8_t len, uint8_t mode ) {
	glcd_bin( cur_x, cur_y, val, len, mode );
}


void glcd_locate( int x, int y ) {
	cur_x = x;
	cur_y = y;
}


uint8_t char_len( char c ) {
	int x=0;
	uint8_t gW, gS, gIS;
	uint8_t startChar = currentFont.startChar;

	gIS = currentFont.interspacePixels;
	gS = currentFont.spacePixels;

	if( c > ' ') {
		gW = pgm_read_byte( &currentFont.charInfo[ c - startChar  ].widthBits );
		x = x + gW + gIS;
	} else x=gS+gIS;


	return x;
}

int text_len( char *s ) {

	int x=0;
	uint8_t gW, gS, gIS;
	uint8_t startChar = currentFont.startChar;

	gIS = currentFont.interspacePixels;
	gS = currentFont.spacePixels;

	while( *s ) {
		if( *s > ' ') {
			gW = pgm_read_byte( &currentFont.charInfo[ *s - startChar  ].widthBits );
			x = x + gW + gIS;
		} else x+=gS+gIS;
		s++;
	}

	return x;
}


int text_len_P( const char *s ) {

	char c;
	int x=0;
	uint8_t gW, gS, gIS;
	uint8_t startChar = currentFont.startChar;

	gIS = currentFont.interspacePixels;
	gS = currentFont.spacePixels;

	while( (c=pgm_read_byte(s)) ) {
		if( c > ' ') {
			gW = pgm_read_byte( &currentFont.charInfo[ c - startChar  ].widthBits );
			x = x + gW + gIS;
		} else x+=gS+gIS;
		s++;
	}

	return x;
}















