/*                  e-gadget.header
 * mk_glcd_text.h
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

#include "../FONTS/mk_fonts.h"

#ifndef MK_GLCD_TEXT_MK_GLCD_TEXT_H_
#define MK_GLCD_TEXT_MK_GLCD_TEXT_H_

// u�ywaj obs�ugi font�w w dw�ch orientacjach T2B i R2L - wi�ksze zu�ycie Flash i czasu
#define USE_TWO_FONTS_BITORIENT			1		//0-disabled, 1-enabled


extern FONT_INFO currentFont;

void setCurrentFont( const FONT_INFO * font );

enum { _left, _center, _right };

extern uint8_t text_align;


void glcd_locate( int x, int y );

void glcd_put_char( int x, int y, char c, uint8_t mode );
void glcd_put_char1( char c, uint8_t mode );

void glcd_puts( int x, int y, char * s, uint8_t mode );
void glcd_puts1( char * s, uint8_t mode );

void glcd_puts_P( int x, int y, const char * s, uint8_t mode );
void glcd_puts1_P( const char * s, uint8_t mode );

void glcd_int( int x, int y, int32_t val, uint8_t mode );
void glcd_int1( int32_t val, uint8_t mode );

void glcd_uint( int x, int y, uint32_t val, uint8_t mode );
void glcd_uint1( uint32_t val, uint8_t mode );

void glcd_hex( int x, int y, uint32_t val, uint8_t mode );
void glcd_hex1( uint32_t val, uint8_t mode );

void glcd_bin( int x, int y, uint32_t val, uint8_t len, uint8_t mode );
void glcd_bin1( uint32_t val, uint8_t len, uint8_t mode );

int text_len( char *s );
uint8_t char_len( char c );
int text_len_P( const char *s );

#endif /* MK_GLCD_TEXT_MK_GLCD_TEXT_H_ */















