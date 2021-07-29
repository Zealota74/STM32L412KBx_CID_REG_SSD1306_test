/*                  e-gadget.header
 * aclock.c
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
#elif defined STM32F3 || defined STM32L4
#include "sw_mcu_conf.h"
#include "../../SW_BOARD/sw_gpio.h"
#include "../../SW_TIMERS/sw_soft_timers.h"
#endif

#include <stdlib.h>

#include "aclock.h"
#include "../mk_glcd_base.h"

static uint8_t ac_xc;
static uint8_t ac_yc;
static uint8_t rad;
static uint8_t last_hh;
static uint8_t last_mm;
static uint8_t last_ss;


static uint8_t px_hh;
static uint8_t py_hh;
static uint8_t px_mm;
static uint8_t py_mm;
static uint8_t px_ss;
static uint8_t py_ss;



static void aBox( uint8_t fkat );
static void iBox( uint8_t x, uint8_t y  );
static void wskazowki( uint8_t angle, uint8_t rad, uint8_t *x, uint8_t *y );
static void tarcza( void );


static uint8_t sin_tab[16] = {0,  27,  54,  79, 104, 128, 150, 171, 190, 201, 221, 233, 243, 250, 254, 255} ;



void aclock_init(uint8_t x, uint8_t y, uint8_t r) {

	ac_xc = x ;
	ac_yc = y ;
	rad = r ;
	last_hh =  r / 2 ;
	last_mm = (r*3) / 4 ;
	last_ss  = last_mm + 1;

	px_hh = px_mm = px_ss = ac_xc ;
	py_hh = py_mm = py_ss = ac_yc ;
		glcd_circle( x,y, r+3, 1);
	tarcza() ;

}

void DisplayTime( uint8_t ahh, uint8_t amm, uint8_t ass ) {
	uint8_t angle ;

	if( ahh == 0 )
           ahh = 12 ;

	glcd_drawLine( ac_xc, ac_yc, px_hh, py_hh, 2 );
	glcd_drawLine( ac_xc, ac_yc, px_mm, py_mm, 2 );
	szachownica = 1;
	glcd_drawLine( ac_xc, ac_yc, px_ss, py_ss, 2 );
	szachownica = 0;

	angle = amm ;
	wskazowki( angle, last_mm, &px_mm, &py_mm ) ;
	glcd_drawLine( ac_xc, ac_yc, px_mm, py_mm, 2 );

	angle = ( ( 5*ahh ) + ( amm/12 ) ) % 60 ;
	wskazowki( angle, last_hh, &px_hh, &py_hh ) ;

	glcd_drawLine( ac_xc, ac_yc, px_hh, py_hh, 2 );

        if( ass != -1)
        {
        	wskazowki( ass, last_ss, &px_ss, &py_ss ) ;

        	szachownica = 1;
        	glcd_drawLine( ac_xc, ac_yc, px_ss, py_ss, 2 );
        	szachownica = 0;
        }

	iBox( ac_xc, ac_yc ) ;
}

void wskazowki( uint8_t angle, uint8_t rad, uint8_t *x, uint8_t *y ) {
	short kwadrat, x_flip, y_flip ;

	kwadrat = angle/15 ;

	switch ( kwadrat ) {
	  case 0 : x_flip = 1 ; y_flip = -1 ; break ;
	  case 1 : angle = abs(angle-30) ; x_flip = y_flip = 1 ; break ;
	  case 2 : angle = angle-30 ; x_flip = -1 ; y_flip = 1 ; break ;
	  case 3 : angle = abs(angle-60) ; x_flip = y_flip = -1 ; break ;
	  default:  x_flip = y_flip =1;
	}
	*x = ac_xc ;
	*y = ac_yc ;
	*x += ( x_flip*(( sin_tab[angle]*rad ) >> 8 )) ;
	*y += ( y_flip*(( sin_tab[15-angle]*rad ) >> 8 )) ;
}

void tarcza( void ) {
	uint8_t hr, x, y ;

	iBox( ac_xc, ac_yc ) ;

	for( hr = 0; hr < 60; hr += 5 ) {
	  if ( !( hr % 15 ) )
		 aBox( hr ) ;
	  else{
		 wskazowki( hr, rad, &x, &y ) ;
	     iBox( x, y ) ;
	  }
	}
}
void iBox( uint8_t x, uint8_t y  ) {

	glcd_drawLine( x, y, x, y, 1 );

	glcd_drawLine( x, y,   x, y, 1 );

	glcd_drawLine( x, y, x, y, 1 );
}

void aBox( uint8_t fkat ) {
	uint8_t kwadrat;
	uint8_t godz;

	kwadrat = ( ( fkat + 7 ) / 15 ) ;
	godz =  fkat == 0 ? 12 : fkat  / 5;

	switch( kwadrat ) {
	  case 0 :
	  case 4 :  // 12
			  glcd_int( ac_xc-6, ac_yc-rad-0, godz, 2 );
			   break;
	  case 1 : // 3
			  glcd_int( ac_xc + rad - 4, ac_yc -3, godz, 2 );
			   break;
	  case 2 : // 6
		  glcd_int( ac_xc-2, ac_yc + rad -6, godz, 2 );
			   break;
	  case 3 : // 9
		  glcd_int( ac_xc- rad+0, ac_yc -3, godz, 2 );
			   break;
	}

}















