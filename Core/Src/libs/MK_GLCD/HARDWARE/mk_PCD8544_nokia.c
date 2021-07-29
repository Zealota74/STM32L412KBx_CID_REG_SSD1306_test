/*                  e-gadget.header
 * mk_PCD8544_nokia.c
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

#ifdef USE_NOKIA_PCD8544

#include "mk_pcd8544_nokia.h"

#ifdef __AVR__
#	include "../../MK_SPI/mk_spi.h"
#elif defined STM32F3
#	include "../../SW_STM_PERIPHS/sw_spi.v2.h"
#endif


uint8_t flipped;



static void nokia_pcd8544_send( uint8_t byte, uint8_t dc ) {

	CS_LO;

    if( dc ) DC_HI;
    else DC_LO;


    spi_xmit_byte( byte );

    CS_HI;
}



// Ustawia kontrast LCD
// Wartosc  w zakresie 30 do 90
void mk_pcd8544_contrast ( uint8_t val ) {

	nokia_pcd8544_send( 0x21, pcd8544_cmd );

    // Ustawienie poziomu kontrastu
	nokia_pcd8544_send( 0x80 | val, pcd8544_cmd );

    // Tryb - horizontal addressing mode.
	nokia_pcd8544_send( 0x20, pcd8544_cmd );
}





void mk_pcd8544_display( void ) {

	if( !can_display ) return;

	nokia_pcd8544_send( 0x80, pcd8544_cmd );
	nokia_pcd8544_send( 0x40, pcd8544_cmd );

	if( !flipped ) {				// display NORMAL
		for ( int i = 0; i < GLCD_BUF_SIZE; i++ ) {
			nokia_pcd8544_send( glcd_buf[ i ], pcd8544_data );
		}
	} else {						// display FLIPPED 180 deg
		SPCR ^= (1<<DORD);

		for ( int i = GLCD_BUF_SIZE-1; i > -1; i-- ) {
			nokia_pcd8544_send( glcd_buf[ i ], pcd8544_data );
		}

		SPCR ^= (1<<DORD);
	}

}



void mk_pcd8544_setPixel( int x, int y, uint8_t mode ) {

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


uint8_t mk_pcd8544_getPixel( int x, int y ) {

	int idx = x + (y/8)*GLCD_WIDTH;
	uint8_t bit = (1<<(y%8));

	uint8_t res = ( glcd_buf[ idx ] & bit ) > 0;

	return res;
}






void mk_pcd8544_init ( void ) {

	spi_init();

	CS_HI;
	DC_DDR |= DC;

#if USE_BACKLIGHT == 1
	BACKLIGHT_DDR |= BACKLIGHT;
	glcd_backlight( 0 );
#endif






#if USE_RST_PIN == 1
	RST_DDR |= RST;

	RST_LO;
	_delay_ms(50);
	RST_HI;
	_delay_ms(50);

#else

	_delay_ms(100);

#endif




    nokia_pcd8544_send( 0x21, pcd8544_cmd ); //* Rozszerzone rozkazy LCD.
    nokia_pcd8544_send( 0xC8, pcd8544_cmd ); //* Ustawienie LCD Vop (Kontrast).
    nokia_pcd8544_send( 0x06, pcd8544_cmd ); //* Ustawienie stabilizacji Temp.
    nokia_pcd8544_send( 0x13, pcd8544_cmd ); //* LCD tryb bias 1:48.
    nokia_pcd8544_send( 0x20, pcd8544_cmd ); //* LCD Standard Commands,Horizontal addressing mode
    nokia_pcd8544_send( 0x0C, pcd8544_cmd ); //* LCD w tryb normal mode.


#if SHOW_DEMO_SCREEN == 0
    glcd_cls();
#endif
    mk_pcd8544_display();

    setCurrentFont( &DefaultFont5x8 );
}



void mk_pcd8544_inverse( uint8_t enable ) {
	if( enable ) nokia_pcd8544_send( 0x0D, pcd8544_cmd );
	else nokia_pcd8544_send( 0x0C, pcd8544_cmd );
}



void mk_pcd8544_flip_vertical( uint8_t flip, uint8_t mirror ) {

	flipped = flip;
}


#endif

















