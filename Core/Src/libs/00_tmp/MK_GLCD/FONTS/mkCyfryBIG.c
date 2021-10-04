/*
 * 	 mkCyfryBIG21x32.c
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


//  Font data for mkCyfryBIG21x32


// Character bitmaps for mkCyfryBIG21x32
const uint8_t mkCyfryBIG21x32Bitmaps[] PROGMEM = {
	0x00, 0xF8, 0x1F, 0x00, 0x80, 0xFF, 0xFF, 0x01,       // char 0  (48)
	0xE0, 0xFF, 0xFF, 0x07, 0xF0, 0xFF, 0xFF, 0x0F,
	0xF8, 0x03, 0x80, 0x3F, 0x3C, 0x00, 0x00, 0x3C,
	0x0E, 0x00, 0x00, 0x70, 0x06, 0x00, 0x00, 0xE0,
	0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0,
	0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xC0,
	0x07, 0x00, 0x00, 0x60, 0x0E, 0x00, 0x00, 0x70,
	0x1C, 0x00, 0x00, 0x3C, 0xF8, 0x01, 0xC0, 0x1F,
	0xF0, 0xFF, 0xFF, 0x0F, 0xE0, 0xFF, 0xFF, 0x07,
	0x80, 0xFF, 0xFF, 0x01, 0x00, 0xFC, 0x1F, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x80,       // char 1  (49)
	0x02, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x80,
	0x06, 0x00, 0x00, 0xC0, 0xFE, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xC0,
	0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80,
	0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x80, 0x80, 0x01, 0x00, 0xC0,       // char 2  (50)
	0xF0, 0x00, 0x00, 0xE0, 0x38, 0x00, 0x00, 0xF0,
	0x1C, 0x00, 0x00, 0xF8, 0x1E, 0x00, 0x00, 0xFC,
	0x0E, 0x00, 0x00, 0xF6, 0x0F, 0x00, 0x00, 0xF3,
	0x0F, 0x00, 0x80, 0xF1, 0x0F, 0x00, 0xE0, 0xF0,
	0x0F, 0x00, 0x70, 0xF0, 0x1F, 0x00, 0x38, 0xF0,
	0x1F, 0x00, 0x1E, 0xF0, 0x3E, 0x00, 0x07, 0xF0,
	0xFE, 0xE0, 0x03, 0xF0, 0xFC, 0xFF, 0x01, 0xF0,
	0xFC, 0x7F, 0x00, 0xF0, 0xF0, 0x1F, 0x00, 0xF0,
	0xE0, 0x07, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x3C,
	0x00, 0x00, 0x00, 0x06,
	0x40, 0x00, 0x00, 0x70, 0x30, 0x00, 0x00, 0x70,       // char 3  (51)
	0x18, 0x00, 0x00, 0xF0, 0x0C, 0x00, 0x00, 0xE0,
	0x06, 0x00, 0x00, 0xE0, 0x06, 0x80, 0x00, 0xC0,
	0x07, 0x80, 0x00, 0xC0, 0x07, 0xC0, 0x00, 0xC0,
	0x07, 0xC0, 0x01, 0xC0, 0x0F, 0xE0, 0x01, 0x40,
	0x0F, 0xF0, 0x03, 0x60, 0x3F, 0xF8, 0x07, 0x70,
	0xFE, 0xCF, 0x1F, 0x3C, 0xFE, 0xC7, 0xFF, 0x1F,
	0xFC, 0x83, 0xFF, 0x0F, 0xF0, 0x00, 0xFF, 0x07,
	0x00, 0x00, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xF8, 0x00,       // char 4  (52)
	0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0xE7, 0x00,
	0x00, 0x80, 0xE3, 0x00, 0x00, 0xE0, 0xE0, 0x00,
	0x00, 0x70, 0xE0, 0x00, 0x00, 0x1C, 0xE0, 0x00,
	0x00, 0x0E, 0xE0, 0x00, 0x80, 0x03, 0xE0, 0x00,
	0xC0, 0x01, 0xE0, 0x00, 0x70, 0x00, 0xE0, 0x00,
	0x38, 0x00, 0xE0, 0x00, 0xFE, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xE0, 0x00,
	0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x00,
	0x00, 0x00, 0xE0, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x00, 0x0C, 0x00, 0x70,       // char 5  (53)
	0x00, 0x0F, 0x00, 0xF0, 0xE0, 0x0F, 0x00, 0xF0,
	0xF8, 0x0F, 0x00, 0xE0, 0x3E, 0x1F, 0x00, 0xC0,
	0x0F, 0x1F, 0x00, 0xC0, 0x0F, 0x1E, 0x00, 0xC0,
	0x0F, 0x3E, 0x00, 0x40, 0x0F, 0x7E, 0x00, 0x40,
	0x0F, 0x7C, 0x00, 0x60, 0x0F, 0xF8, 0x00, 0x30,
	0x0F, 0xF8, 0x03, 0x18, 0x0F, 0xF0, 0x0F, 0x0C,
	0x0F, 0xE0, 0xFF, 0x07, 0x0F, 0x80, 0xFF, 0x03,
	0x01, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0x7F, 0x00, 0x00, 0xF0, 0xFF, 0x03,       // char 6  (54)
	0x00, 0xFC, 0xFF, 0x0F, 0x00, 0xFF, 0xFF, 0x1F,
	0x80, 0xFF, 0x00, 0x3F, 0xC0, 0x47, 0x00, 0x78,
	0xE0, 0x61, 0x00, 0x70, 0xF0, 0x20, 0x00, 0xE0,
	0x38, 0x30, 0x00, 0xC0, 0x1C, 0x30, 0x00, 0xC0,
	0x0C, 0x30, 0x00, 0xC0, 0x0E, 0x30, 0x00, 0xC0,
	0x06, 0x70, 0x00, 0xC0, 0x02, 0xF0, 0x00, 0x60,
	0x03, 0xE0, 0x01, 0x70, 0x03, 0xE0, 0x07, 0x3C,
	0x01, 0xC0, 0xFF, 0x1F, 0x01, 0x80, 0xFF, 0x0F,
	0x01, 0x00, 0xFF, 0x07, 0x00, 0x00, 0xFC, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0xC0, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,       // char 7  (55)
	0x3E, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
	0x0F, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
	0x0F, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x80,
	0x0F, 0x00, 0x00, 0xF0, 0x0F, 0x00, 0x00, 0xFE,
	0x0F, 0x00, 0xC0, 0x7F, 0x0F, 0x00, 0xF8, 0x0F,
	0x0F, 0x00, 0xFE, 0x01, 0x0F, 0xC0, 0x3F, 0x00,
	0x0F, 0xF8, 0x07, 0x00, 0x0F, 0xFF, 0x01, 0x00,
	0xEF, 0x3F, 0x00, 0x00, 0xFF, 0x07, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
	0x07, 0x00, 0x00, 0x00,
	0xE0, 0x03, 0xC0, 0x07, 0xF8, 0x0F, 0xF0, 0x1F,       // char 8  (56)
	0xFC, 0x1F, 0xF8, 0x3F, 0xFE, 0x3F, 0xFC, 0x7F,
	0x0E, 0x7F, 0x1E, 0x78, 0x07, 0xFC, 0x06, 0xF0,
	0x03, 0xF8, 0x03, 0xE0, 0x03, 0xF0, 0x01, 0xC0,
	0x03, 0xE0, 0x03, 0xC0, 0x03, 0xC0, 0x07, 0xC0,
	0x03, 0xE0, 0x0F, 0xC0, 0x07, 0xB0, 0x1F, 0xC0,
	0x0E, 0x3C, 0x3F, 0x60, 0xFE, 0x1F, 0x7E, 0x70,
	0xFC, 0x0F, 0xFC, 0x3F, 0xF8, 0x07, 0xF8, 0x3F,
	0xF0, 0x01, 0xF0, 0x1F, 0x00, 0x00, 0xC0, 0x07,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x80, 0x3F, 0x00, 0x00, 0xE0, 0xFF, 0x00, 0x80,       // char 9  (57)
	0xF0, 0xFF, 0x01, 0x80, 0xF8, 0xFF, 0x03, 0x80,
	0x3C, 0xE0, 0x07, 0x80, 0x0E, 0x80, 0x07, 0xC0,
	0x06, 0x00, 0x0F, 0x40, 0x03, 0x00, 0x0E, 0x60,
	0x03, 0x00, 0x0C, 0x60, 0x03, 0x00, 0x0C, 0x30,
	0x03, 0x00, 0x0C, 0x38, 0x03, 0x00, 0x0C, 0x1C,
	0x07, 0x00, 0x04, 0x0F, 0x0E, 0x00, 0x86, 0x07,
	0x1E, 0x00, 0xE2, 0x03, 0xFC, 0x00, 0xFE, 0x01,
	0xF8, 0xFF, 0xFF, 0x00, 0xF0, 0xFF, 0x3F, 0x00,
	0xC0, 0xFF, 0x0F, 0x00, 0x00, 0xFE, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       // char :  (58)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x03,
	0xF0, 0x00, 0x80, 0x07, 0xF0, 0x00, 0x80, 0x07,
	0x60, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

// descriptors
const FONT_CHAR_INFO mkCyfryBIG21x32Descriptors[] PROGMEM = {
	{ 21,0 },	// char 0  (48)
	{ 21,84 },	// char 1  (49)
	{ 21,168 },	// char 2  (50)
	{ 21,252 },	// char 3  (51)
	{ 21,336 },	// char 4  (52)
	{ 21,420 },	// char 5  (53)
	{ 21,504 },	// char 6  (54)
	{ 21,588 },	// char 7  (55)
	{ 21,672 },	// char 8  (56)
	{ 21,756 },	// char 9  (57)
	{ 21,840 },	// char :  (58)
};

// Font information
const FONT_INFO mkCyfryBIG PROGMEM = {
	0,
	32,	// Caracter height
	'0',	// Start character
	2,	// number of pixels of interspace between characters
	2,	// number of pixels of space character
	mkCyfryBIG21x32Descriptors,	// number of pixels of space character
	mkCyfryBIG21x32Bitmaps,	// number of pixels of space character
	0,	// (Pointer) Font filename saved on SD card or 0 (null) otherwise  )
	0,// bits and byte orientation 0-T2B, 1-L2R
};