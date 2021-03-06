/*
 * 	 RockwellCondensed14x32.c
 *
 * Font generated by ATNEL - SFG (part of PixelFactory)
 * http://atnel.pl/pixel-factory.html
 * (C) 2015 by Atnel. Miros�aw Karda�
 */
#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#elif defined STM32F3 || defined STM32L4
#include "sw_mcu_conf.h"
#include "../../SW_BOARD/sw_gpio.h"
#include "../../SW_TIMERS/sw_soft_timers.h"
#define PROGMEM
#define pgm_read_byte	*
#define _delay_ms		delay_ms
#endif


#include "mk_fonts.h"


//  Font data for RockwellCondensed14x32


// Character bitmaps for RockwellCondensed14x32
const uint8_t RockwellCondensed14x32Bitmaps[] PROGMEM = {
	0x00, 0xFF, 0xFF, 0x00, 0xF0, 0xFF, 0xFF, 0x0F,       // char 0  (48)
	0xFC, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 0xFF, 0x7F,
	0x3E, 0x00, 0x00, 0x7C, 0x1F, 0x00, 0x00, 0xF0,
	0x0F, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0xF0,
	0x0F, 0x00, 0x00, 0xF0, 0x3E, 0x00, 0x00, 0x7C,
	0xFE, 0xFF, 0xFF, 0x7F, 0xFC, 0xFF, 0xFF, 0x3F,
	0xF0, 0xFF, 0xFF, 0x0F, 0x00, 0xFF, 0xFF, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       // char 1  (49)
	0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0xF0,
	0x3C, 0x00, 0x00, 0xF0, 0xFC, 0xFF, 0xFF, 0xFF,
	0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xF0,
	0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xF0, 0x07, 0x00, 0xFE, 0xFC, 0x07, 0x80, 0xFF,       // char 2  (50)
	0xFE, 0x07, 0xE0, 0xFF, 0xFE, 0x07, 0xF8, 0xFF,
	0x1F, 0x00, 0xFC, 0xF1, 0x0F, 0x00, 0x7E, 0xF0,
	0x0F, 0x80, 0x3F, 0xF0, 0x0F, 0xC0, 0x0F, 0xF0,
	0x1F, 0xF0, 0x07, 0xF0, 0xFE, 0xFF, 0xC3, 0xFF,
	0xFE, 0xFF, 0xC0, 0xFF, 0xFC, 0x7F, 0xC0, 0xFF,
	0xF0, 0x0F, 0xC0, 0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x01, 0xE0, 0x07,       // char 3  (51)
	0xFC, 0x01, 0xE0, 0x3F, 0xFE, 0x01, 0xE0, 0x7F,
	0xFF, 0x01, 0xE0, 0x7F, 0x1F, 0xE0, 0x01, 0xF8,
	0x0F, 0xE0, 0x01, 0xF0, 0x0F, 0xE0, 0x03, 0xF0,
	0x1F, 0xF0, 0x07, 0xF8, 0xFE, 0xFF, 0xFF, 0x7F,
	0xFE, 0x3F, 0xFF, 0x7F, 0xFC, 0x3F, 0xFE, 0x1F,
	0xE0, 0x0F, 0xF8, 0x07, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x7F, 0x00,       // char 4  (52)
	0x00, 0xC0, 0x7F, 0x00, 0x00, 0xF8, 0x77, 0x00,
	0x00, 0xFE, 0x70, 0x00, 0xC0, 0x3F, 0x70, 0x00,
	0xF0, 0x07, 0x70, 0xF0, 0xFE, 0x01, 0x70, 0xF0,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x70, 0xF0, 0x00, 0x00, 0x70, 0xF0,
	0x00, 0x00, 0xC0, 0x07, 0xFE, 0xFF, 0xC1, 0x1F,       // char 5  (53)
	0xFF, 0xFF, 0xC1, 0x3F, 0xFF, 0xFF, 0xC1, 0x7F,
	0xFF, 0xFF, 0x03, 0xF8, 0x0F, 0xF0, 0x00, 0xF0,
	0x0F, 0x70, 0x00, 0xF0, 0x0F, 0x78, 0x00, 0xF0,
	0x7F, 0xF8, 0x00, 0xF8, 0x7F, 0xF8, 0xFF, 0x7F,
	0x7F, 0xF0, 0xFF, 0x3F, 0x7F, 0xE0, 0xFF, 0x1F,
	0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x80, 0xFF, 0xFF, 0x01, 0xF8, 0xFF, 0xFF, 0x1F,       // char 6  (54)
	0xFC, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 0xFF, 0x7F,
	0x1F, 0xC0, 0x01, 0xF8, 0x0F, 0xE0, 0x00, 0xF0,
	0x0F, 0xE0, 0x00, 0xF0, 0x1F, 0xE0, 0x00, 0xF0,
	0xFF, 0xE1, 0x01, 0xFC, 0xFE, 0xE1, 0xFF, 0x7F,
	0xFC, 0xC1, 0xFF, 0x3F, 0xF0, 0x81, 0xFF, 0x1F,
	0x00, 0x00, 0xFE, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,       // char 7  (55)
	0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xF0,
	0xFF, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0xFC, 0xFF,
	0x0F, 0xF8, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF,
	0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xF0,
	0xFF, 0x03, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0x00,
	0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xF0, 0x0F, 0xFC, 0x07, 0xFC, 0x3F, 0xFF, 0x1F,       // char 8  (56)
	0xFE, 0xFF, 0xFF, 0x3F, 0xFE, 0xFF, 0xFF, 0x7F,
	0x1F, 0xF0, 0x03, 0xF8, 0x0F, 0xE0, 0x01, 0xF0,
	0x0F, 0xE0, 0x01, 0xF0, 0x0F, 0xE0, 0x01, 0xF0,
	0x1F, 0xF0, 0x01, 0xF0, 0xFE, 0xFF, 0x03, 0xFC,
	0xFE, 0xFF, 0xFF, 0x7F, 0xFC, 0x3F, 0xFF, 0x3F,
	0xF0, 0x0F, 0xFE, 0x1F, 0x00, 0x00, 0xF8, 0x07,
	0xC0, 0x7F, 0x00, 0x00, 0xF8, 0xFF, 0x81, 0x07,       // char 9  (57)
	0xFC, 0xFF, 0x83, 0x3F, 0xFE, 0xFF, 0x87, 0x7F,
	0x3F, 0xC0, 0x87, 0x7F, 0x0F, 0x00, 0x07, 0xF8,
	0x0F, 0x00, 0x07, 0xF0, 0x0F, 0x00, 0x07, 0xF0,
	0x0F, 0x00, 0x03, 0xF0, 0x1F, 0x80, 0x03, 0x7C,
	0xFE, 0xFF, 0xFF, 0x7F, 0xFC, 0xFF, 0xFF, 0x3F,
	0xF8, 0xFF, 0xFF, 0x0F, 0x80, 0xFF, 0xFF, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       // char :  (58)
	0xF0, 0x01, 0xE0, 0x03, 0xF0, 0x01, 0xE0, 0x03,
	0xF0, 0x01, 0xE0, 0x03, 0xF0, 0x01, 0xE0, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// descriptors
const FONT_CHAR_INFO RockwellCondensed14x32Descriptors[] PROGMEM = {
	{ 14,0 },	// char 0  (48)
	{ 14,56 },	// char 1  (49)
	{ 14,112 },	// char 2  (50)
	{ 14,168 },	// char 3  (51)
	{ 14,224 },	// char 4  (52)
	{ 14,280 },	// char 5  (53)
	{ 14,336 },	// char 6  (54)
	{ 14,392 },	// char 7  (55)
	{ 14,448 },	// char 8  (56)
	{ 14,504 },	// char 9  (57)
	{ 8,560 },	// char :  (58)
};

// Font information
const FONT_INFO RockwellCondensed14x32 PROGMEM = {
	0,
	32,	// Caracter height
	'0',	// Start character
	2,	// number of pixels of interspace between characters
	5,	// number of pixels of space character
	RockwellCondensed14x32Descriptors,	// number of pixels of space character
	RockwellCondensed14x32Bitmaps,	// number of pixels of space character
	0,	// (Pointer) Font filename saved on SD card or 0 (null) otherwise  )
	0,// bits and byte orientation 0-T2B, 1-L2R
};
