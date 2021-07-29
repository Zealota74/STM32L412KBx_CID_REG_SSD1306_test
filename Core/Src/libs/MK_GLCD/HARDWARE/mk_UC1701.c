/*                  e-gadget.header
 * mk_UC1701.c
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
#include "../../SW_BOARD/sw_gpio.h"
#include "../../SW_TIMERS/sw_soft_timers.h"
#define PROGMEM
#define pgm_read_byte	*
#define _delay_ms		delay_ms
#endif

#include <string.h>




#include "../mk_glcd_base.h"

#ifdef USE_UC1701

#include "mk_UC1701.h"




#ifdef __AVR__
#	include "../../MK_SPI/mk_spi.h"
#elif defined STM32F3
#	include "../../SW_STM_PERIPHS/sw_spi.v2.h"
#endif


static uint8_t flipped;




//***** wyl�lij komend� ***********
static void mk_uc1701_cmd( uint8_t cmd ) {


	CS_HI;
	DC_LO;
	CS_LO;

	spi_xmit_byte( cmd );

	CS_HI;



}




//************************************************************
// podstawowa funkcja do wy�wietlania pojedynczego pixela
void mk_uc1701_setPixel( int x, int y, uint8_t mode ) {

	if( (x < 0) || (x > GLCD_WIDTH-1) || (y < 0) || (y > GLCD_HEIGHT-1 ) ) return;

	if( viewport.active ) {
		if ((x < viewport.left) || (x > viewport.right) || (y < viewport.top) || (y > viewport.bottom ))
			return;
	}

	//------------ bytes - vertical T2B -----------------------------------------------------
	int idx = x + (y/8)*GLCD_WIDTH;
	uint8_t bit = (1<<(y%8));

	if( !mode ) glcd_buf[idx] &= ~(bit);			// 0 - zga� pixel
	else if( 1 == mode ) glcd_buf[idx] |= (bit); 	// 1 - zapal pixel
	else glcd_buf[idx] ^= (bit);					// 2 - zmie� na przeciwny (XOR)

	can_display = 1;

}

uint8_t mk_uc1701_getpixel( int x, int y ) {

	int idx = x + (y/8)*GLCD_WIDTH;
	uint8_t bit = (1<<(y%8));

	uint8_t res = ( glcd_buf[ idx ] & bit ) > 0;

	return res;
}

//************ wy�lij bufor RAM do sterownika ************
void mk_uc1701_display( void ) {

	if( !can_display ) return;

	uint16_t idx = 0;
	for(uint8_t i = 0; i < 8; i++ ) {

		mk_uc1701_cmd( 0xB0 + i );
		mk_uc1701_cmd( 0x10 );
		if( !flipped ) mk_uc1701_cmd( 0x04 );
		else mk_uc1701_cmd( 0x00 );

		DC_HI;
		CS_LO;
		for(uint8_t j = 0; j < GLCD_WIDTH; j++ ) {
			spi_xmit_byte( glcd_buf[ idx++ ] );
		}
		CS_HI;
	}
}




//************* INICJALIZACJA sterownika SSD1306
void mk_uc1701_init( void ) {

#if USE_BACKLIGHT == 1
	BACKLIGHT_DDR |= BACKLIGHT;
#endif

	_delay_ms( 100 );

#if USE_RST_PIN == 1
	RST_LO;
	_delay_ms(10);
	RST_HI;
	_delay_ms(10);
#endif

	_delay_ms(10);

	spi_init();

	DC_DDR |= DC;

	mk_uc1701_cmd(0xE2);  	//System Reset
	mk_uc1701_cmd(0x40); 	// Set display start line to 0
	mk_uc1701_cmd(0xA1); 	//Set SEG Direction
	mk_uc1701_cmd(0xC0); 	//Set COM Direction
	mk_uc1701_cmd(0xA2); 	//Set Bias = 1/9
	mk_uc1701_cmd(0x2C);  	//Boost ON
	mk_uc1701_cmd(0x2E); 	//Voltage Regular On
	mk_uc1701_cmd(0x2F); 	//Voltage Follower On
	mk_uc1701_cmd(0xF8); 	//Set booster ratio to
	mk_uc1701_cmd(0x00); 	//4x
	mk_uc1701_cmd(0x23); 	//Set Resistor Ratio = 3

	mk_uc1701_cmd(0x81);
	mk_uc1701_cmd( COG_CONTRAST ); 	//Set Electronic Volume (contrast)

	mk_uc1701_cmd(0xAC);//Set Static indicator off
	mk_uc1701_cmd(0x00);
	mk_uc1701_cmd(0xA6); // Disable inverse
	mk_uc1701_cmd(0xAF); //Set Display Enable

	mk_uc1701_cmd(0xA5); //display all points
	mk_uc1701_cmd(0xA4); //normal display


#if SHOW_DEMO_SCREEN == 0
	    glcd_cls();
#endif

	    mk_uc1701_display();

	    setCurrentFont( &DefaultFont5x8 );
}



void mk_uc1701_inverse( uint8_t enable ) {
	if( enable ) mk_uc1701_cmd( 0xA7 );
	else mk_uc1701_cmd( 0xA6 );
}



void mk_uc1701_flip_vertical( uint8_t flip, uint8_t mirror ) {
	if( !flip ) {
		mk_uc1701_cmd( 0xC0 );		// normal
		flipped = 0;

		if( !mirror ) mk_uc1701_cmd( 0xA1 );
		else {
			mk_uc1701_cmd( 0xA0 );
			flipped = 1;
		}


	} else {

		mk_uc1701_cmd( 0xC8 );
		flipped = 1;

		if( !mirror ) mk_uc1701_cmd( 0xA0 );
		else {
			mk_uc1701_cmd( 0xA1 );
			flipped = 0;
		}
	}
}





#endif		// USE_UC1701
















