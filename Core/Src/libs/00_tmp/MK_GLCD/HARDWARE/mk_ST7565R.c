/*                  e-gadget.header
 * mk_ST7565R.c
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

#ifdef USE_COG_ST7565R

#include "mk_ST7565R.h"




#ifdef __AVR__
#	include "../../MK_SPI/mk_spi.h"
#elif defined STM32F3
#	include "../../SW_STM_PERIPHS/sw_spi.v2.h"
#endif


uint8_t mirrored;



//***** wyl�lij komend� ***********
static void st7565_cmd( uint8_t cmd ) {

	CS_HI;
	DC_LO;
	CS_LO;

	spi_xmit_byte( cmd );

	CS_HI;
}

//***********  wy�lij dane *************
static void st7565_data( uint8_t dat ) {

	CS_HI;
	DC_HI;
	CS_LO;

	spi_xmit_byte( dat );

	CS_HI;
}


//********** Ustawienie kontrastu wy�wietlacza od 0 do 255
void mk_st7565_contrast( uint8_t val ) {

	st7565_cmd(CMD_SET_VOLUME_FIRST);
	st7565_cmd( val & 0x3f );

}



//************ wy�lij bufor RAM do sterownika ************
void mk_st7565_display( void ) {

	if( !can_display ) return;

	int idx=0;

	#if GLCD_HEIGHT == 64
		uint8_t pgmax = 8;
	#endif
	#if GLCD_HEIGHT == 32
		uint8_t pgmax = 4;
	#endif

	st7565_cmd( CMD_RMW );
	for (uint8_t i = 0; i < pgmax; i++) {

		st7565_cmd( CMD_SET_PAGE | i );

		if( mirrored ) st7565_cmd( CMD_SET_COLUMN_LOWER | (0x0 & 0xf) | 4);
		else st7565_cmd( CMD_SET_COLUMN_LOWER | (0x0 & 0xf));

		st7565_cmd( CMD_SET_COLUMN_UPPER | ((0x0 >> 4) & 0xf) );

		for (uint8_t j = 0; j < GLCD_WIDTH; j++) st7565_data( glcd_buf[ idx++ ] );
	}

}




//************************************************************
// podstawowa funkcja do wy�wietlania pojedynczego pixela
void mk_st7565_setPixel(int x, int y, uint8_t mode) {

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


uint8_t mk_st7565_getPixel( int x, int y ) {

	int idx = x + (y/8)*GLCD_WIDTH;
	uint8_t bit = (1<<(y%8));

	uint8_t res = ( glcd_buf[ idx ] & bit ) > 0;

	return res;
}



//************* INICJALIZACJA sterownika SSD1306
void mk_st7565_init( void ) {




#if USE_BACKLIGHT == 1
	BACKLIGHT_DDR |= BACKLIGHT;
#endif

	glcd_backlight( 0 );

	_delay_ms(100);

	spi_init();

	DC_DDR |= DC;

#if USE_RST_PIN == 1
	RST_DDR |= RST;

	RST_LO;
	_delay_ms(50);
	RST_HI;
	_delay_ms(50);
#endif


	st7565_cmd(CMD_SET_BIAS_9);
	st7565_cmd(CMD_SET_ADC_REVERSE);
	st7565_cmd(CMD_SET_COM_NORMAL);
	st7565_cmd(CMD_SET_DISP_START_LINE);
	st7565_cmd(0x2c );
	_delay_ms(10);
	st7565_cmd(0x2e);
	_delay_ms(10);
	st7565_cmd(0x2f);
	_delay_ms(10);
	st7565_cmd(CMD_SET_RESISTOR_RATIO | 0x6);

	st7565_cmd(0x26);

	st7565_cmd(CMD_DISPLAY_ON);

	mk_st7565_contrast( COG_CONTRAST );

#if SHOW_DEMO_SCREEN == 0
	glcd_cls();
#endif
	mk_st7565_display();

	setCurrentFont( &DefaultFont5x8 );

}


void mk_st7565_inverse( uint8_t enable ) {
	if( enable ) st7565_cmd( CMD_SET_DISP_REVERSE );
	else st7565_cmd( CMD_SET_DISP_NORMAL );
}


void mk_st7565_flip_vertical( uint8_t flip, uint8_t mirror ) {
	if( !flip ) {

		st7565_cmd( 0xC0 );		// normal

		if( !mirror ) st7565_cmd( 0xA1 );
		else st7565_cmd( 0xA0 );

		mirrored = mirror;

	} else {

		st7565_cmd( 0xC8 );		// flipped

		if( !mirror ) st7565_cmd( 0xA0 );
		else st7565_cmd( 0xA1 );

		mirrored = !mirror;

	}

}





#endif

















