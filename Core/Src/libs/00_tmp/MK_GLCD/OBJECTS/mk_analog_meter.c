/*                  e-gadget.header
 * mk_analog_meter.c
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
#include <math.h>
#define PROGMEM
#define pgm_read_byte	*
#define _delay_ms		delay_ms
const char * PSTR(const char * stringPTR) { return (const char *)stringPTR; }
#endif

#include <stdlib.h>
#include <stdio.h>


#include "../mk_glcd_base.h"

#include "mk_analog_meter.h"


static int ltx;
static uint16_t osx, osy;


#define TICK_STEP	5



TANALOGMETER_CONFIG ameter;







void default_percent_needle_callback( void ) {


	static int lval;
	uint8_t ftype;

	if( ameter.sval < lval ) ftype = fx_ld_down;
	else ftype = fx_ld_up;

	TCOUNTERPARAMSINIT cp;


#ifdef USE_NOKIA_PCD8544
	cp.align = _center;
	cp.len1 = 3;
	cp.len2 = 1;
	cp.atr1 = ' ';
	cp.atr2 = ' ';
	cp.font1 = &DefaultFont5x8;
	cp.font2 = &MkMINI3x6;
	cp.interspace_px1 = 2;
	cp.interspace_px2 = 1;
	cp.leading_zero = 0;
	cp.as_clock = 0;
	cp.speed = 1;
	cp.delay = 1;
	glcd_fx_counter( GLCD_WIDTH, GLCD_HEIGHT-12, 1, ameter.sval, 7, ftype, PSTR("^C"), PSTR("."), &cp );
#else
	cp.align = _center;
	cp.len1 = 3;
	cp.len2 = 1;
	cp.atr1 = ' ';
	cp.atr2 = ' ';
	cp.font1 = &FixedSys5x12;
	cp.font2 = &DefaultFont5x8;
	cp.interspace_px1 = 2;
	cp.interspace_px2 = 1;
	cp.leading_zero = 1;
	cp.as_clock = 0;
	cp.speed = 1;
	cp.delay = 1;
	glcd_fx_counter( GLCD_WIDTH, GLCD_HEIGHT-12, 2, ameter.sval, 0, ftype, PSTR("\xB0""C"), PSTR(","), &cp );

	setCurrentFont( &DefaultFont5x8 );
	glcd_int( 0,GLCD_HEIGHT-8, ameter.last_pval, 1 );
	glcd_puts1_P( PSTR("%  "), 1 );
#endif



	lval = ameter.sval;
}



void ameter_init( int min, int max, uint8_t fract_pos, const FONT_INFO * font, uint8_t speed, uint8_t delay, int8_t off_range, void (*needle_callback)(void) ) {

	ameter.fract_pos		= fract_pos;

	ameter_set_scale( ASCALE );
	ameter_set_xoffset( XOFFSET );
	ameter_set_yoffset( YOFFSET );
	ameter_set_needlen();

	ameter.min 				= min;

	if( ameter.fract_pos ) ameter.max = max*(ameter.fract_pos*10);
	else ameter.max	= max;

	ameter.amax 			= max;
	ameter.speed			= speed;
	ameter.delay			= delay;
	ameter.off_range		= off_range;

#if USE_DEFAULT_PERCENT_CALLBACK == 1
	if( !needle_callback )	ameter.needle_callback = default_percent_needle_callback;
	else ameter.needle_callback	= needle_callback;
#else
	ameter.needle_callback	= needle_callback;
#endif

	ameter.font				= font;

	ameter.direct_draw		= 0;

	ameter.last_pval		= -10;
	ameter.pval				= 0;

	ameter.value			= 0;

	ameter.dotted_needle	= 1;


}

void ameter_set_val( int val ) {

	ameter.value = val;
	ameter.pval = map16( ameter.value, ameter.min,ameter.max,  0,100 );

}


void AMETER_EVENT( void ) {

	static uint32_t cnt;

	if( ameter.direct_draw ) {
		ameter.last_pval = ameter.pval;
		ameter.sval = ameter.value;
		draw_Needle( 1, 0 );
	}

	else if( !( cnt % ameter.delay ) ) {


		if( ameter.sval != ameter.value ) {
			if (ameter.sval < ameter.value) {
				ameter.sval += ameter.speed;
				if( ameter.speed > 1 ) {
					if( abs(ameter.sval-ameter.value) < ameter.speed ) ameter.sval = ameter.value;
				}
			} else {
				ameter.sval -= ameter.speed;
				if( ameter.speed > 1 ) {
					if( abs(ameter.sval-ameter.value) < ameter.speed ) ameter.sval = ameter.value;
				}
			}
		}

		ameter.pval = map16( ameter.sval, ameter.min,ameter.max,  0,100 );

		if( ameter.last_pval != ameter.pval ) {
			if (ameter.last_pval < ameter.pval) {
				ameter.last_pval += ameter.speed;
				if( ameter.speed > 1 ) {
					if( abs(ameter.last_pval-ameter.pval) < ameter.speed ) ameter.last_pval = ameter.pval;
				}
			} else {
				ameter.last_pval -= ameter.speed;
				if( ameter.speed > 1 ) {
					if( abs(ameter.last_pval-ameter.pval) < ameter.speed ) ameter.last_pval = ameter.pval;
				}
			}
		}


		draw_Needle( 1, 0 );
	}

	cnt++;
}







void draw_analog_meter( void ) {

	FONT_INFO fcopy;

      for (int i = -50; i < 51; i += TICK_STEP) {

		  int tl = 10;

		  float sx = cos((i - 90) * 0.0174532925);
		  float sy = sin((i - 90) * 0.0174532925);
		  uint16_t x0 = sx * (ameter.scale + tl) + ameter.xoffset;
		  uint16_t y0 = sy * (ameter.scale + tl) + ameter.yoffset;
		  uint16_t x1 = sx * ameter.scale + ameter.xoffset;
		  uint16_t y1 = sy * ameter.scale + ameter.yoffset;

		  szachownica = 0;

		  if( ameter.off_range > 0 ) {
			  if (i > (abs(ameter.off_range))-50 ) szachownica = 1;
		  } else {
			  if (i < (abs(ameter.off_range))-50 ) szachownica = 1;
		  }


		    if (i % (100/4) != 0) tl = 5;

		    x0 = sx * (ameter.scale + tl) + ameter.xoffset;
		    y0 = sy * (ameter.scale + tl) + ameter.yoffset;
		    x1 = sx * ameter.scale + ameter.xoffset;
		    y1 = sy * ameter.scale + ameter.yoffset;

		    // Draw tick
		    glcd_drawLine(x0, y0, x1, y1, 1);


		    if (i % (100/4) == 0) {


		    	memcpy( &fcopy, &currentFont, sizeof(currentFont) );
		    	setCurrentFont( ameter.font );


		    	uint8_t hl = currentFont.heightPixels/2;

		    	char lcz[5][6];
		    	char bf[2];

		    	int am = ameter.amax - ameter.min;
		    	int a;


		    	itoa( ameter.min, &lcz[0][0], 10 );
		    	uint8_t sl1 = text_len( &lcz[0][0] );



		    	a = (am*10)/4;
		    	a = a + ameter.min*10;
		    	itoa( a/10, &lcz[1][0], 10 );
		    	uint8_t sl2 = text_len( &lcz[1][0] );
		    	if( (a%10) ) {
		    		strcat( &lcz[1][0], "," );
		    		itoa( abs(a%10), bf, 10 );
		    		strcat( &lcz[1][0], bf );
		    		sl2 = text_len( &lcz[1][0] );
		    	}

		    	a = ((am*10)/2);
		    	a = a + ameter.min*10;
		    	itoa( a/10, &lcz[2][0], 10 );
		    	uint8_t sl3 = text_len( &lcz[2][0] );
		    	if( (a%10) ) {
		    		strcat( &lcz[2][0], "," );
		    		itoa( abs(a%10), bf, 10 );
		    		strcat( &lcz[2][0], bf );
		    		sl3 = text_len( &lcz[2][0] );
		    	}

		    	a = (((am*10)/4)*3);
		    	a = a + ameter.min*10;
		    	itoa( a/10, &lcz[3][0], 10 );
		    	uint8_t sl4 = text_len( &lcz[3][0] );
		    	if( (a%10) ) {
		    		strcat( &lcz[3][0], "," );
		    		itoa( abs(a%10), bf, 10 );
		    		strcat( &lcz[3][0], bf );
		    		sl4 = text_len( &lcz[3][0] );
		    	}

		    	a = am*10;
		    	a = a + ameter.min*10;
		    	itoa( a/10, &lcz[4][0], 10 );
		    	uint8_t sl5 = text_len( &lcz[4][0] ) - currentFont.interspacePixels;


		      x0 = sx * (ameter.scale + tl ) + ameter.xoffset;
		      y0 = sy * (ameter.scale + tl + hl*2+2) + ameter.yoffset;
		      switch (i / (100/4)) {
		        case -2: glcd_puts( x0-sl1/2, y0-hl, &lcz[0][0], 1 ); break;
		        case -1: glcd_puts( x0-sl2/2, y0-1, &lcz[1][0], 1 ); break;
		        case 0: glcd_puts( x0-sl3/2+2, y0+1, &lcz[2][0], 1 ); break;
		        case 1: glcd_puts( x0-sl4/3+2, y0-1, &lcz[3][0], 1 ); break;
		        case 2: glcd_puts( x0-sl5/2+2, y0-hl, &lcz[4][0], 1 ); break;
		      }

		    }

		    memcpy( &currentFont, &fcopy, sizeof(currentFont) );


		  sx = cos((i + TICK_STEP - 90) * 0.0174532925);
		  sy = sin((i + TICK_STEP - 90) * 0.0174532925);
		  x0 = sx * ameter.scale + ameter.xoffset;
		  y0 = sy * ameter.scale + ameter.yoffset;

		  if (i < 50) glcd_drawLine(x0, y0, x1, y1, 1);
	  }

}





void draw_Needle( uint8_t need_solid, uint8_t need_change_val ) {



	if (ameter.last_pval < -7) ameter.last_pval = -7;
	if (ameter.last_pval > 105) ameter.last_pval = 105;

    float sdeg = map16(ameter.last_pval, -7, 105, -147, -35);

    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    float tx = tan((sdeg+90) * 0.0174532925);

    szachownica = 0;
	glcd_drawLine(ameter.xoffset + NEEDOFSET * ltx, ameter.yoffset - NEEDOFSET, osx, osy, 0);


//	if( ameter.needle_callback ) ameter.needle_callback();

	uint8_t nl = ameter.needlen;

    ltx = tx;
    if( ameter.dotted_needle && (ameter.last_pval > 100 || ameter.last_pval < 0) ) {
		osx = sx * (nl +10) + ameter.xoffset;
		osy = sy * (nl +10) + ameter.yoffset;
    } else {
		osx = sx * nl + ameter.xoffset;
		osy = sy * nl + ameter.yoffset;
    }

    if( need_solid ) szachownica = 0;
    else szachownica = 1;

    if( ameter.off_range != 0 ) {

    	if( ameter.off_range > 0 ) {
    		if( ameter.last_pval > abs(ameter.off_range) ) szachownica = 1; else szachownica = 0;
    	} else {
    		if( ameter.last_pval < abs(ameter.off_range) ) szachownica = 1; else szachownica = 0;
    	}
    } else szachownica = 0;


    glcd_drawLine(ameter.xoffset + NEEDOFSET * ltx, ameter.yoffset - NEEDOFSET, osx, osy, 2);

    if( ameter.needle_callback ) ameter.needle_callback();

    szachownica = 0;

}














