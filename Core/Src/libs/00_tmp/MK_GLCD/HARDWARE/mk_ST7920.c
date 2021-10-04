/*                  e-gadget.header
 * mk_ST7920.c
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

#ifdef USE_ST7920

#include "mk_ST7920.h"

#ifdef __AVR__
#	include "../../MK_SPI/mk_spi.h"
#elif defined STM32F3
#	include "../../SW_STM_PERIPHS/sw_spi.v2.h"
#endif



static void mk_st7920_cmd( uint8_t cmd ) {

    spi_xmit_byte( 0xF8 );			// Sync.    - 1  1  1  1  1  0  0  0  RW=0,RS=0
    spi_xmit_byte( cmd & 0xF0 );	// HighData - D7 D6 D5 D4 0  0  0  0
    spi_xmit_byte( cmd << 4 );		// LowData  - D3 D2 D1 D0 0  0  0  0

    _delay_us(72);
}

static void mk_st7920_dat( uint8_t dat ) {

    spi_xmit_byte( 0xFA );			// Sync.    - 1  1  1  1  1  0  1  0 RW=0 RS=1
    spi_xmit_byte( dat & 0xF0 );	// HighData - D7 D6 D5 D4 0  0  0  0
    spi_xmit_byte( dat << 4 );		// LowData  - D3 D2 D1 D0 0  0  0  0
    if( 0x01 == dat ) _delay_ms( 1.6 );
    else _delay_us(72);
}




void mk_st7920_setPixel( int x, int y, uint8_t mode ) {

	if( (x < 0) || (x > GLCD_WIDTH-1) || (y < 0) || (y > GLCD_HEIGHT-1 ) ) return;

	if( viewport.active ) {
		if ((x < viewport.left) || (x > viewport.right) || (y < viewport.top) || (y > viewport.bottom ))
			return;
	}

	//------------ bytes - horizontal R2L -----------------------------------------------------
	int idx = (x/8) + (y*(GLCD_WIDTH/8));
	uint8_t bit = (1<<(7-(x%8)));

	if( !mode ) glcd_buf[idx] &= ~(bit);			// 0 - zga� pixel
	else if( 1 == mode ) glcd_buf[idx] |= (bit); 	// 1 - zapal pixel
	else glcd_buf[idx] ^= (bit);					// 2 - zmie� na przeciwny (XOR)

	can_display = 1;


}


uint8_t mk_st7920_getPixel( int x, int y ) {

	int idx = (x/8) + (y*(GLCD_WIDTH/8));
	uint8_t bit = (1<<(7-(x%8)));

	uint8_t res = ( glcd_buf[ idx ] & bit ) > 0;

	return res;
}


void mk_st7920_init( void ) {

#if USE_BACKLIGHT == 1
	BACKLIGHT_DDR |= BACKLIGHT;
#endif

	_delay_ms( 100 );

	spi_init();
	CS_HI;


	mk_st7920_cmd( FUNC_SET_BAS ); 			// Basic Function Set
	_delay_us(120);
	mk_st7920_cmd( FUNC_SET_BAS ); 			// Repeat Function Set
	_delay_us(120);
	mk_st7920_cmd( DISPLAY_CMD ); 			// Display
	_delay_us(120);
	mk_st7920_cmd( ST7920_CMD_CLEAR ); 		// Clear Display
	_delay_us(120);
	mk_st7920_cmd( ENTRY_MODE_SET ); 		// Set Entry Mode
	_delay_us(120);
	mk_st7920_cmd( FUNC_SET_EXT );
	_delay_us(120);

    glcd_cls();

	mk_st7920_display();

	setCurrentFont( &DefaultFont5x8 );

}







void mk_st7920_display( void ) {

    int i,j;

    mk_st7920_cmd( FUNC_SET_EXT + 2 );

    for(i=0; i<32; i++) {
        // Vertical-Adr
    	mk_st7920_cmd( 0x80 + i );
        // Horizontal-Adr
    	mk_st7920_cmd( 0x80 );

        // upper part of Display
        for( j=0; j<8; j++ ) {
        	mk_st7920_dat( glcd_buf[(i*16) + (j*2)] );
        	mk_st7920_dat( glcd_buf[(i*16) + (j*2) +1] );
        }
        // lower part of Display
        for( j=0; j<8; j++ ) {
        	mk_st7920_dat( glcd_buf[(i*16)+512 + (j*2)] );
        	mk_st7920_dat( glcd_buf[(i*16)+512 + (j*2) +1] );
        }

    }

    mk_st7920_cmd( 0x30 );
}



void mk_st7920_flip_vertical( uint8_t flip, uint8_t mirror ) {

	// not implemented

}

void mk_st7920_inverse( uint8_t enable ) {

	// not implemented

}





#endif  // #ifdef USE_ST7920










