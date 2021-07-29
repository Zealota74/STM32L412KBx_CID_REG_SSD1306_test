/*
 * bitmaps.h
 *
 *  Created on: 22 paź 2015
 *      Author: simon
 */

#ifndef LIBS_BITMAPS_H_
#define LIBS_BITMAPS_H_

#ifdef __AVR_
#include <avr/pgmspace.h>
#else
#define PROGMEM
#include <stdint.h>
#endif

uint8_t mem_128x64_buf[1024];
extern const unsigned char pic[];
extern const uint8_t picture_1[192] PROGMEM;
extern const uint8_t temp_icon[192] PROGMEM;

#endif /* LIBS_BITMAPS_H_ */
