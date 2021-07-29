/*
 * bitmaps.c
 *
 *  Created on: 30 paź 2015
 *      Author: simon
 */

#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif
#include "bitmaps.h"


uint8_t mem_128x64_buf[1024] = {
//uint8_t mem_128x64_buf[1024] = {
/**		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x3F,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0x7F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x3F,0x3F,0x1F,0x1F,0x0F,0x0F,0x07,0x07,
		0x03,0x03,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF9,0xF0,
		0xC0,0x00,0x00,0x01,0x03,0x0F,0x3F,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x3F,0x1F,0x0F,0x0F,0x0F,
		0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x07,0x07,0x03,
		0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xF7,0xE3,0x81,0x00,0x00,0x03,0x07,0x3F,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFC,0xC0,0x00,0x00,0x00,0x01,0x0F,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x0F,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x80,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xE0,0xC0,0x80,0x80,0x80,
		0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF,0xFF,0xFF,0xFF,0x80,0x80,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFB,0xF1,0xE0,0xC0,0xC0,0xE0,0xF8,0xFE,0xFF,0xFF,0xFF,0xFF,
		0xFF,0x3F,0x0F,0x01,0x00,0x00,0x00,0xC0,0xF8,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,
		0xFE,0xFE,0xFC,0xFC,0xF8,0xF8,0xF8,0xF0,0xF0,0xE0,0xE0,0xE0,0xC0,0xC0,0x80,0x80,
		0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE7,0xC3,
		0x80,0x00,0x80,0xC0,0xF0,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFC,0xFC,0xF8,0xF8,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
**/
//		0xFF,0x81,0x81,0x81,0xFF
};

const uint8_t temp_icon[] PROGMEM = {
		0x04,0x00,0x00,
		0x44,0x20,0x00,
		0x20,0x40,0x00,
		0x06,0x00,0x00,
		0x09,0x00,0x00,
		0x10,0x80,0x00,
		0xD0,0xB0,0x00,
		0x10,0x80,0x00,
		0x09,0x00,0x00,
		0x06,0x00,0x00,
		0x20,0x40,0x00,
		0x42,0x00,0x00,
		0x02,0x18,0x00,
		0x00,0x42,0x7f,
		0x00,0x00,0x00,
		0x00,0x81,0x00,
		0x00,0x99,0x00,
		0x00,0xBD,0x7C,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x7C,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x7C,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x7F,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x7C,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x7C,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x7C,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x00,
		0x00,0xBD,0x7F,
		0x00,0xBD,0x00,
		0x01,0x3C,0x80,
		0x02,0x7E,0x40,
		0x00,0xFF,0x00,
		0x04,0xFF,0x20,
		0x05,0xFF,0xA0,
		0x04,0xFF,0x20,
		0x00,0xFF,0x00,
		0x02,0x7E,0x40,
		0x01,0x3C,0x80,
		0x00,0xC3,0x00,
		0x00,0x7E,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
};

