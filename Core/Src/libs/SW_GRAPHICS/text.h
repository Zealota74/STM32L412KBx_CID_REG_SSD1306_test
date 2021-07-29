/*
 * text.h
 *
 *  Created on: 06.04.2017
 *      Author: simon
 */

#ifndef LIBS_SW_GRAPHICS_TEXT_H_
#define LIBS_SW_GRAPHICS_TEXT_H_

#include "colors.h"
#include "fonts_common.h"
#include "fonts_pl.h"

typedef struct {
	int16_t x, y;				// Text position
	uint16_t stringLength;		// Tekst length

	wchar_t 	*str;			// Pointer ot text beginning
	FONT_INFO	*fontPtrPGM;	// Font parameters structure pointer

	uint8_t onChange;			// Text changing event flag
	int16_t lastPosX, lastPosY;
} T_STRING;


void graphic_set_string( T_STRING *string, wchar_t *str, int16_t x, int16_t y, const FONT_INFO *fontPtrPGM );

uint16_t graphic_puts_RAM( volatile T_DISPLAY *buffer, T_STRING *string, uint8_t textSize, uint32_t color, uint32_t bg, const T_GAMMA *gamma );
uint16_t graphic_puts_int_RAM( volatile T_DISPLAY *buffer, T_STRING *string, int data, uint8_t textSize, uint32_t color, uint32_t bg, const T_GAMMA *gamma );
void graphic_puts_hex_RAM( volatile T_DISPLAY *buffer, T_STRING *string, uint16_t data, uint8_t textSize, uint32_t color, uint32_t bg, const T_GAMMA *gamma );

void graphic_clear_last_text( volatile T_DISPLAY *buffer, T_STRING *string, uint32_t bg, const T_GAMMA *gamma );


void graphic_puts_sqr_int_RAM( volatile T_DISPLAY *buffer, T_STRING *string, int data, uint32_t color, uint32_t bg, const T_GAMMA *gamma );

void graphic_spec_char_RAM( volatile T_DISPLAY *buffer, T_STRING *string, int data, uint8_t charLine, uint32_t color, uint32_t bg, const T_GAMMA *gamma );

uint8_t graphic_char_to_wide( wchar_t* dest, char* source );

//void graphic_puts_float_RAM( volatile T_DISPLAY *buffer, T_STRING *string, float data, uint8_t textSize,
//							uint32_t color, uint32_t bg, const T_GAMMA *gamma );

void text_floating_string( volatile T_DISPLAY * buffer );
void text_scrolling_text( volatile T_DISPLAY * buffer, uint32_t color, int8_t dir );
void display_integer( volatile T_DISPLAY * buffer, uint32_t color, int data );


/******** Test function *********************/
void test_text_param( FONT_INFO * font );
void test_text_display_number( uint8_t number );
/********************************************/

void TEXT_display_number( int16_t x, int16_t y, int16_t number );
void TEXT_display_string( int16_t x, int16_t y, wchar_t *  str );
void TEXT_display_float( int16_t x, int16_t y, float number );

#endif /* LIBS_SW_GRAPHICS_TEXT_H_ */
