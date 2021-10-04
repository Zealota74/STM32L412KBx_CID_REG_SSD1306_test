/*                  e-gadget.header
 * mk_scr_saver.c
 *
 *  Created on: 2019-06-15
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
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#elif defined STM32F3 || defined STM32L4
#include "sw_mcu_conf.h"
#include "../../SW_BOARD/sw_gpio.h"
#include "../../SW_TIMERS/sw_soft_timers.h"
#define PROGMEM
#define pgm_read_byte	*
#define _delay_ms		delay_ms
#endif

#include <stdlib.h>
#include <stdio.h>


#include "../mk_glcd_base.h"



#include "mk_scr_saver.h"


volatile uint8_t screen_saver_emergency_exit;

static volatile uint8_t * scr_soft_timer;
static uint8_t (*scr_saver_break_callback)( void );
static void (*scr_saver_onexit_callback)( void );

/* 	W celu uruchomienia Screen Savera nale�y bezwzgl�dnie przekaza�:
 *
 * 		1. *scr_tmr - wska�nik do wolnego timera programowego
 * 		2. break_callback - wska�nik do funkcji przerywaj�cej ScrenSaver
 *
 * 		poza tym mo�na ale nie jest to wymagane, przekaza� tak�e wska�nik do funkcji, kt�ra ma si� wykona�
 * 		po zako�czeniu pracy ScreenSavera
 *
 * 		2. onexit_callback
 *
 * 		Uwaga! zako�czenie pracy Screen Savera nast�pi gdy funkcja zwrotna break_callback przeka�e jako rezultat
 * 		warto�� r�n� od zera. Funkcja ta mo�e s�u�y� tak�e do realizacji w tle pracy ScreenSavera innych krytycznie
 * 		wa�nych proces�w jak np zdarzenie obs�ugi UART, I2C, SPI czy podczerwieni lub klawiatury i innych.
 *
 */
void register_scr_saver( volatile void * scr_tmr, uint8_t (*break_callback)(void), void (*onexit_callback)(void) ) {
	scr_soft_timer = scr_tmr;
	scr_saver_break_callback = break_callback;
	scr_saver_onexit_callback = onexit_callback;
}




void glcd_screen_saver1( uint8_t cnt ) {

	void reload_buble( TBUBLE * bl ) {
		bl->x = rand() % 127;
		bl->r = 1 + (rand() % 8);
		bl->y = GLCD_HEIGHT + bl->r;
		bl->yoffset = 1 + ( rand() % 10 );
		bl->xoffset = ( rand() % 3 );
	}


	if( !scr_soft_timer ) return;

	uint8_t auto_exit = 0;
	int auto_exit_cnt = 400;

	if( !cnt ) {
		cnt = 5;
		auto_exit = 1;
	}


	TBUBLE bubles[cnt];
	for( uint8_t i=0; i<cnt; i++ ) {
		bubles[i].x = rand() % 127;
		bubles[i].r = 1 + (rand() % 10);
		bubles[i].y = GLCD_HEIGHT + bubles[i].r;
		bubles[i].yoffset = 1 + ( rand() % 10 );
		bubles[i].xoffset = ( rand() % 3 );
	}

	uint8_t exit_result = 0;

	while(1) {

		if( screen_saver_emergency_exit ) break;

		if( scr_saver_break_callback ) exit_result = scr_saver_break_callback();
		if( exit_result ) break;

		if( !(*scr_soft_timer) ) {
			*scr_soft_timer = 0;

			if( auto_exit ) {
				if( auto_exit_cnt ) auto_exit_cnt--;
				if( !auto_exit_cnt ) break;
			}

			for( uint8_t i=0; i<cnt; i++ ) {
				glcd_fillCircle( bubles[i].lx, bubles[i].ly, bubles[i].r, 0 );
				bubles[i].y -= bubles[i].yoffset;
				bubles[i].x += bubles[i].xoffset;
				bubles[i].xoffset *= -1;
				glcd_circle( bubles[i].x, bubles[i].y, bubles[i].r, 1 );
				bubles[i].lx = bubles[i].x;
				bubles[i].ly = bubles[i].y;
				if( bubles[i].y + bubles[i].r < 0 ) reload_buble( &bubles[i] );
			}
			glcd_display();

		}
	}

	if( scr_saver_onexit_callback ) scr_saver_onexit_callback();

}











