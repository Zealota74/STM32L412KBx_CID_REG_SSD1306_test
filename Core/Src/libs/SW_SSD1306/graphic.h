/*
 * graphic.h
 *
 *  Created on: 28 paź 2015
 *      Author: simon
 */

#ifndef LIBS_GRAPHIC_H_
#define LIBS_GRAPHIC_H_

#include "fonts_common.h"

#define SSD1306	0
#define KS102	1
#define SSD1322	2

#define DISPLAY_DRIVER	SSD1306

#define BLACK	0
#define WHITE	1

#if DISPLAY_DRIVER==SSD1322
	#define _width	SSD1322_WIDTH
	#define _height	SSD1322_HEIGHT
#elif DISPLAY_DRIVER==SSD1306
	#define _width	SSD1306_WIDTH
	#define _height	SSD1306_HEIGHT
#endif


/********************** ******************************************************/
#define swap(a, b) { int16_t t = a; a = b; b = t; }


//extern	FONT_INFO CurrentFont;
void	graphic_set_current_font (FONT_INFO * font, FONT_INFO *currentFont);


// Definicje funkcji
uint8_t graphic_draw_char_RAM(int16_t x, int16_t y, wchar_t charCode, uint8_t color, uint16_t bg, uint8_t size, FONT_INFO charPtrPGM);
uint8_t graphic_puts_RAM  	(int x, int y, wchar_t *str, uint8_t textSize, uint8_t color, uint8_t bg, FONT_INFO charPtrPGM);
uint8_t graphic_puts_RAM_P	(int x, int y, const wchar_t *str, uint8_t textSize, uint8_t color, uint8_t bg, FONT_INFO charPtrPGM);

uint8_t graphic_puts_int_RAM(int x, int y, 		int data, uint8_t textSize, uint8_t color, uint8_t bg, FONT_INFO charPtrPGM);
uint8_t graphic_puts_hex_RAM(int x, int y, uint16_t data, uint8_t textSize, uint8_t color, uint8_t bg, FONT_INFO charPtrPGM);

void	graphic_draw_fast_Hline_RAM	(int16_t x, int16_t y, int16_t w, uint8_t color);
void	graphic_fill_rect_RAM		(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
void 	graphic_draw_bitmap_PGM( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color );

void	graphic_fill_screen_RAM(uint8_t color);


#endif /* LIBS_GRAPHIC_H_ */
