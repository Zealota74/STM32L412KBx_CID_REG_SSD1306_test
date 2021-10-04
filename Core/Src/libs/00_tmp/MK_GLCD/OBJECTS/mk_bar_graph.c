/*                  e-gadget.header
 * mk_bar_graph.c
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

#include "mk_bar_graph.h"




TBARPROPERTIES	bar_properties;










void glcd_drawbar( int x, int y, int width, int height, int8_t percent ) {

	uint8_t md = 1 + bar_properties.checker * 2;
	uint8_t ins = bar_properties.inspace * 2;
	if( height < 3 ) height = 3;
	if( width < 3 ) width = 3;
	if( abs(percent) > 100 ) percent = 100;
	if( height < 5 ) ins = 0;

	szachownica = bar_properties.outline_checker;
	glcd_drawRect( x,y, width, height, 1 );
	szachownica = 0;
	glcd_fillRect( x+1,y+1, width-3, height-2, 0 );

	if( !bar_properties.vertical ) {
		if( bar_properties.down_ticks ) {
			uint8_t tl = bar_properties.tick_length;
			uint8_t td = width/4;
			uint8_t ax = x+td;
			glcd_drawLine( x,y+height-1, x, y+height+tl-1, 1 );
			glcd_drawLine( ax,y+height-1, ax, y+height+tl/2-1, 1 );
			ax += td;
			glcd_drawLine( ax,y+height-1, ax, y+height+tl-1, 1 );
			ax += td;
			glcd_drawLine( ax,y+height-1, ax, y+height+tl/2-1, 1 );
			glcd_drawLine( x+width-1,y+height-1, x+width-1, y+height+tl-1, 1 );
		}
		if( bar_properties.up_ticks ) {
			uint8_t tl = bar_properties.tick_length;
			uint8_t td = width/4;
			uint8_t ax = x+td;
			glcd_drawLine( x,y-tl, x, y, 1 );
			glcd_drawLine( ax,y-tl/2, ax, y, 1 );
			ax += td;
			glcd_drawLine( ax,y-tl, ax, y, 1 );
			ax += td;
			glcd_drawLine( ax,y-tl/2, ax, y, 1 );
			glcd_drawLine( x+width-1,y-tl, x+width-1, y, 1 );
		}
	}

	if( !ins ) ins = 1;

	uint8_t wp=0, hp=0;



	if( !bar_properties.vertical ) {
		wp = width - 1  - ins*ins - ins%2;
		wp = (percent*wp) / 100UL;
		hp = height - ins*2;
		if( wp ) glcd_fillRect( x+ins, y+ins, wp, hp, md );
		if( bar_properties.show_text && height > currentFont.heightPixels-currentFont.down_space+1+ins*2 ) {

			char abuf[5];
			mkitoa( percent, abuf,3,0 );
			strcat( abuf, "%" );

			uint8_t ax = (width)/2 - text_len(abuf)/2 + currentFont.interspacePixels + 1;
			uint8_t ay = (height)/2 - (currentFont.heightPixels-currentFont.down_space)/2;

			if( !bar_properties.checker ) glcd_puts( x+ax,y+ay, abuf, 2 );
		}
	} else {
		hp = height - 0 - ins*ins - ins%2;
		hp = (percent*hp) / 100UL;
		wp = width - ins*2-1;
		if( hp ) glcd_fillRect( x+ins, (height+y-ins)-hp, wp, hp, md );
	}


}








