/*
 * 7segment.c
 *
 *  Created on: 8 gru 2015
 *      Author: simon
 */


#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#include "../fonts_common.h"

const char SevenSegmentBitmaps[] PROGMEM =
{
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0xFF,0xFE,0x00,
	0x01,0xFF,0xFF,0x00,
	0x03,0xFF,0xFF,0x80,
	0x01,0xFF,0xFF,0x60,
	0x0C,0xFF,0xFE,0xF0,
	0x1E,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3E,0x00,0x00,0x78,
	0x38,0x00,0x00,0x18,
	0x20,0x00,0x00,0x08,
	0x00,0x00,0x00,0x00,
	0x20,0x00,0x00,0x00,
	0x38,0x00,0x00,0x18,
	0x3E,0x00,0x00,0x78,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x3F,0x00,0x01,0xF8,
	0x1E,0x00,0x00,0xF0,
	0x0C,0xFF,0xFE,0x60,
	0x01,0xFF,0xFF,0x00,
	0x03,0xFF,0xFF,0x80,
	0x01,0xFF,0xFF,0x00,
	0x00,0xFF,0xFE,0x00,
	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,  // 0

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // 1
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x00,0xFF,0xFE,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x78,0x01,0xFF,0xFE,0x18,0x03,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x27,0xFF,0xFF,0xC0,0x39,0xFF,0xFF,0x00,0x3E,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x0C,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // 2
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x00,0xFF,0xFE,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x78,0x01,0xFF,0xFE,0x18,0x03,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xC0,0x01,0xFF,0xFF,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // 3
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x0C,0x00,0x00,0xF0,0x1E,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3E,0x00,0x00,0x78,0x39,0xFF,0xFE,0x18,0x23,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xC0,0x01,0xFF,0xFF,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // 4
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x0C,0xFF,0xFE,0x00,0x1E,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x39,0xFF,0xFE,0x00,0x23,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xC0,0x01,0xFF,0xFF,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // 5
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x0C,0xFF,0xFE,0x00,0x1E,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x39,0xFF,0xFE,0x00,0x23,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,0xE0,0x27,0xFF,0xFF,0xC0,0x39,0xFF,0xFF,0x18,0x3E,0x00,0x00,0x78,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x1E,0x00,0x00,0xF0,0x0C,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // 6
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x00,0xFF,0xFE,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // 7
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x0C,0xFF,0xFE,0xF0,0x1E,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3E,0x00,0x00,0x78,0x39,0xFF,0xFE,0x18,0x23,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x27,0xFF,0xFF,0xC0,0x39,0xFF,0xFF,0x18,0x3E,0x00,0x00,0x78,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x1E,0x00,0x00,0xF0,0x0C,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // 8
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x0C,0xFF,0xFE,0xF0,0x1E,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3E,0x00,0x00,0x78,0x39,0xFF,0xFE,0x18,0x23,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xC0,0x01,0xFF,0xFF,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // 9

};

const FONT_CHAR_INFO SevenSegmentDescriptors[] PROGMEM =
{
		{32,0},
		{32,200},
		{32,400},
		{32,600},
		{32,800},
		{32,1000},
		{32,1200},
		{32,1400},
		{32,1600},
		{32,1800},
};

// Font information for Microsoft Sans Serif 8pt
const FONT_INFO SevenSegmentFontInfo PROGMEM =
{
	50,										// Character height
	L'0',									// Start character
	1,										// number of pixels of interspace between characters
	4,										// number of pixels of space character
	SevenSegmentDescriptors,	// Character descriptor array
	SevenSegmentBitmaps,		// Character bitmap array
	0,										// (Pointer) Font filename saved on SD card or 0 (null) otherwise
	sizeof(SevenSegmentDescriptors) / sizeof(SevenSegmentDescriptors[0]),

};

