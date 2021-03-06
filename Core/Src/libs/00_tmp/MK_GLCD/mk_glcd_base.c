/*                  e-gadget.header
 * mk_glcd_base.c
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
#include <avr/eeprom.h>
#include <util/delay.h>
#elif defined STM32F3 || defined STM32L4
#include "sw_mcu_conf.h"
#include "../SW_BOARD/sw_gpio.h"
#include "../SW_TIMERS/sw_soft_timers.h"
#define PROGMEM
#define pgm_read_byte	*
#define _delay_ms		delay_ms
#endif
#include <string.h>
#include <stdlib.h>


#include "mk_glcd_base.h"


int cur_x;
int cur_y;

uint8_t szachownica;
uint8_t can_display;

TVIEWPORT viewport;

uint8_t glcd_buf[ GLCD_BUF_SIZE ] = {

#if SHOW_DEMO_SCREEN == 1

#ifdef GLCD_RES_84_48
		// res: 84x48   - 504 bytes
		0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF0,0xF0,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
		0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,
		0xF8,0xF8,0xF8,0xF8,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
		0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
		0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x30,0xF0,0xC0,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0x7F,0x1F,0x07,0xC3,0xE3,0x83,0x0F,0x7F,0xFF,0xFF,0xFF,0xF3,0xF3,0xF3,0x03,
		0x03,0x03,0xF3,0xF3,0xF3,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0x7C,
		0xF8,0xE0,0x80,0x00,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0x8C,0x8C,
		0x8C,0x8C,0x00,0x00,0x00,0x00,0xFC,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xCF,0xC3,0xC0,0xF0,0xF8,0xFB,0xFB,0xFB,0xF8,0xE0,0xC3,0xCF,0xFF,
		0xFF,0xFF,0xFF,0xC0,0xC0,0xC0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,
		0x3F,0x3F,0x3F,0x00,0x01,0x07,0x1F,0x3E,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x3F,
		0x3F,0x3F,0x31,0x31,0x31,0x31,0x00,0x00,0x00,0x00,0x3F,0x3F,0x3F,0x30,0x30,0x30,
		0x30,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x03,0x1F,0x3F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
		0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
		0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
		0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x60,0x38,0x1F,0x07,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0xF0,0x30,0xC0,0x00,0x00,0xF0,0x00,0xC0,0x20,0x10,0x10,0x10,0x20,
		0xC0,0x00,0xF0,0x80,0x40,0x20,0x10,0x00,0x10,0xF0,0x10,0x00,0x00,0xC0,0x30,0x30,
		0xC0,0x00,0x00,0x00,0x00,0x00,0xF0,0x10,0x10,0x10,0xE0,0x00,0xC0,0x20,0x10,0x10,
		0x10,0x10,0x00,0xF0,0x10,0x10,0x10,0x20,0xC0,0x00,0x60,0x90,0x90,0x90,0x60,0x00,
		0xF0,0x90,0x90,0x90,0x10,0x00,0x80,0x40,0x20,0xF0,0x00,0x00,0x80,0x40,0x20,0xF0,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x00,0x03,0x0C,0x0F,0x00,0x03,0x04,
		0x08,0x08,0x08,0x04,0x03,0x00,0x0F,0x01,0x02,0x04,0x08,0x00,0x08,0x0F,0x08,0x00,
		0x0E,0x03,0x02,0x02,0x03,0x0E,0x00,0x00,0x00,0x00,0x0F,0x01,0x01,0x01,0x00,0x00,
		0x03,0x04,0x08,0x08,0x08,0x08,0x00,0x0F,0x08,0x08,0x08,0x04,0x03,0x00,0x07,0x08,
		0x08,0x08,0x07,0x00,0x04,0x08,0x08,0x08,0x07,0x00,0x01,0x01,0x01,0x0F,0x01,0x00,
		0x01,0x01,0x01,0x0F,0x01,0x00,0x00,0x00,
#endif

#ifdef GLCD_RES_128_64
		0x00,0x00,0x06,0x0A,0x12,0xFE,0x12,0x0A,0x06,0x80,0x00,0xC0,0x00,0xE0,0x00,0xF0,
		0x00,0x00,0x00,0x00,0x00,0x00,0x88,0x50,0xFE,0x22,0x54,0x88,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFE,0x06,0x06,0xFE,0xFE,0x06,
		0x06,0xFE,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xF8,0x00,0x00,0x00,0x00,0x10,
		0x18,0x1C,0xFE,0xFE,0x1C,0x18,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0xFE,0x06,0xEE,0xDE,0xDE,0xEE,0x06,0xFE,0x06,0x76,0x76,0x8E,0xFE,
		0xF6,0xB6,0xB6,0x4E,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x08,0x0E,0x02,
		0xFA,0xFA,0xFA,0x02,0xFA,0xFA,0xFA,0x02,0xFA,0xFA,0xFA,0x02,0xFE,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x02,0x01,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
		0x03,0x03,0x03,0x00,0x00,0x00,0x04,0x0C,0x1C,0x3F,0x3F,0x1C,0x0C,0x04,0x00,0x00,
		0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x0F,0x0C,0x0F,0x0F,0x0F,0x0F,0x0C,0x0F,0x0C,0x0F,0x0F,0x0F,0x0F,
		0x0D,0x0D,0x0D,0x0E,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x02,0x0E,0x08,
		0x0B,0x0B,0x0B,0x08,0x0B,0x0B,0x0B,0x08,0x0B,0x0B,0x0B,0x08,0x0F,0x00,0x00,0x00,
		0x00,0x00,0x1F,0x15,0x11,0x00,0x0E,0x11,0x01,0x2E,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x60,
		0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x04,0x04,0x04,0x04,0x78,0x00,0x00,0x78,0x84,
		0x84,0x84,0x84,0x00,0x00,0x00,0x00,0xF8,0xF8,0x18,0x38,0x78,0xF8,0xF8,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x66,0x66,0x66,
		0x66,0x66,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x66,
		0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x10,0x10,0x10,0x10,0x0F,0x00,0x00,0x00,0x10,
		0x10,0x10,0x10,0x0F,0x00,0x00,0x00,0x3F,0x3F,0x30,0x38,0x3C,0x3E,0x3F,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x66,
		0x66,0x66,0x66,0x66,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x66,0x66,0x66,
		0x66,0x66,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x66,0x66,0x66,0x66,0x66,
		0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x60,0x60,0x60,0x60,0x60,0x00,0x00,
		0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x66,0x66,
		0x66,0x66,0x66,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x60,0x60,0x60,0x60,
		0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x00,
		0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x80,0x80,
		0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0xFC,0xFF,0x83,0x83,0x83,0x83,0xFF,0xFC,0x00,0x00,0x03,0x03,0x03,
		0xFF,0xFF,0x03,0x03,0x03,0x00,0x00,0xFF,0xFF,0x0E,0x38,0xE0,0x80,0xFF,0xFF,0x00,
		0x00,0xFF,0xFF,0x63,0x63,0x63,0x03,0x03,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x1F,
		0x00,0x00,0x3F,0x20,0x20,0x20,0x00,0x00,0x3F,0x24,0x24,0x20,0x00,0x00,0x3F,0x20,
		0x20,0x20,0x1F,0x00,0x00,0x00,0xE0,0xE0,0x20,0x20,0x20,0xE0,0xC0,0x00,0x00,0xE0,
		0xE0,0x20,0x20,0x20,0x00,0x00,0xE0,0xE0,0xC0,0x80,0x00,0x80,0xC0,0xE0,0xE0,0x00,
		0x00,0xC0,0xE0,0x20,0x20,0x20,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x0F,0x0F,0x01,0x01,0x01,0x01,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,
		0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x03,0x0F,0x0F,0x00,
		0x00,0x0F,0x0F,0x0C,0x0C,0x0C,0x0C,0x0C,0x00,0x00,0x0F,0x0F,0x0C,0x0C,0x0C,0x0C,
		0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x3F,0x20,0x20,0x20,0x3F,0x1F,0x00,0x00,0x3F,
		0x3F,0x22,0x22,0x20,0x00,0x00,0x3F,0x3F,0x00,0x01,0x03,0x01,0x00,0x3F,0x3F,0x00,
		0x00,0x1F,0x3F,0x20,0x20,0x20,0x3F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#endif	// GLCD_RES_128_64

#ifdef GLCD_RES_128_32
		0x00,0x00,0xE0,0xF0,0xF8,0xF8,0xFC,0xFC,0x7C,0x7C,0x7C,0xFC,0xFC,0xFC,0x7C,0x7C,
		0x7C,0x7C,0x7C,0xFC,0xFC,0x04,0x04,0x84,0x84,0x84,0x04,0x04,0x84,0x84,0x04,0x04,
		0x84,0x84,0x84,0x84,0x84,0x04,0x04,0x84,0x84,0x04,0x04,0x04,0x08,0x18,0xE0,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x40,0x60,0x20,0x30,0x30,0x30,
		0x30,0x30,0x30,0x20,0x60,0x60,0xC0,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0xE0,0xC0,
		0xC0,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xDF,0xC3,0xE0,0xF6,0xF0,0xC1,0xCF,0xFF,0xFF,0xFF,
		0xC0,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x3F,0x3F,0x07,0x0E,0x1C,0x3F,0x3F,0x00,0x00,
		0x3F,0x3F,0x24,0x24,0x20,0x00,0x00,0x3F,0x3F,0x20,0x20,0x00,0x00,0x00,0xFF,0x00,
		0x00,0x00,0x80,0x80,0xE0,0xF8,0x8E,0x83,0x81,0x80,0x80,0x80,0xFC,0xFC,0x30,0x10,
		0x18,0x0C,0x0C,0x04,0x06,0x02,0x03,0x01,0x03,0x0E,0xFC,0xF0,0x00,0x00,0x00,0x00,
		0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF,0xFF,0xFF,0x00,
		0x01,0x01,0x03,0x03,0x07,0x07,0x0E,0x0C,0x1C,0x1C,0x38,0x30,0x70,0x70,0xE0,0xE0,
		0xC0,0xC0,0xFF,0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,
		0x00,0x00,0x01,0x07,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x8F,0x4F,
		0x4F,0x4F,0x8F,0x0F,0x0F,0xC8,0x08,0x08,0x08,0x08,0xC8,0x48,0x48,0x48,0x48,0x08,
		0xC8,0x48,0x48,0x48,0x88,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x04,0x06,0x01,0x00,
		0x00,0x00,0x01,0x01,0x07,0x3F,0x71,0xC1,0x81,0x01,0x01,0x01,0x3F,0x3F,0x0C,0x18,
		0x18,0xF0,0xF0,0xF8,0xF8,0xE0,0xC0,0x80,0xC0,0x70,0x3F,0x0F,0x00,0x00,0x00,0x00,
		0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,0xFF,0xFF,0x00,
		0x80,0x80,0xC0,0xC0,0xE0,0xE0,0x70,0x70,0x38,0x38,0x1C,0x1C,0x0E,0x0E,0x07,0x07,
		0x03,0x03,0xFF,0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x10,0x20,
		0x20,0x20,0x10,0x0F,0x00,0x3F,0x20,0x20,0x20,0x00,0x3F,0x22,0x22,0x22,0x20,0x00,
		0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x06,0x06,0x0C,0x0C,0x0C,0x0C,
		0x0C,0x0C,0x0C,0x0C,0x04,0x06,0x03,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x0F,0x07,0x03,
		0x03,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#endif

#endif		// SHOW_DEMO_SCREEN
};





































