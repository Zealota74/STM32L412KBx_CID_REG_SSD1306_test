/*                  e-gadget.header
 * mk_SSD1306.c
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

#ifdef USE_SSD1306

#include "mk_SSD1306.h"




#if USE_I2C == 1
#	ifdef __AVR__
#		include "../../MK_I2C/mk_i2c.h"
#	elif defined STM32F3 || defined STM32L4
//#		include "../../SW_STM_PERIPHS/sw_i2c_base_v1.h"
#		include "../../sw_i2c_simple.h"
//#		define i2c_start		sw_i2c_start
//#		define i2c_stop			sw_i2c_stop
//#		define i2c_write		sw_i2c_write_byte
#		define i2c_write_buf	sw_i2c_write_block
#		define i2cSetBitrate	sw_i2c_set_bitrate
uint8_t TWBR = 0;
#	endif
#else
#	ifdef __AVR__
#		include "../../MK_SPI/mk_spi.h"
#	elif defined STM32F3
#		include "../../SW_STM_PERIPHS/sw_spi.v2.h"
#	endif
#endif


#if USE_I2C == 1

static uint8_t twbr_oled;
static uint8_t twbr_copy;

static void i2c_ssd1306_send( uint8_t ctrl, uint8_t dc ) {

	twbr_copy = TWBR;
	if( TWBR != twbr_oled ) TWBR = twbr_oled;

#ifdef STM32L412xx

	sw_i2c_write_reg( GLCD_I2C_ADDRESS, ctrl, dc );

	#else
    i2c_start();
    i2c_write( GLCD_I2C_ADDRESS );
	i2c_write( ctrl );
    i2c_write( dc );
    i2c_stop();
#endif

    TWBR = twbr_copy;
}
#endif


//***** wyl�lij komend� ***********
static void mk_ssd1306_cmd( uint8_t cmd ) {

#if USE_I2C == 1

	i2c_ssd1306_send( 0x00, cmd );


#endif

#if USE_I2C == 0

	CS_HI;
	DC_LO;
	CS_LO;

	spi_xmit_byte( cmd );

	CS_HI;

#endif

}



////***********  wy�lij dane *************
//static void mk_ssd1306_data( uint8_t dat ) {
//
//#if USE_I2C == 1
//
//    i2c_ssd1306_send( 0x40, dat );
//
//#endif
//
//}


//************************************************************
// podstawowa funkcja do wy�wietlania pojedynczego pixela
void mk_ssd1306_setPixel( int x, int y, uint8_t mode ) {

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


uint8_t mk_ssd1306_getPixel( int x, int y ) {

	int idx = x + (y/8)*GLCD_WIDTH;
	uint8_t bit = (1<<(y%8));

	uint8_t res = ( glcd_buf[ idx ] & bit ) > 0;

	return res;
}





//************ wy�lij bufor RAM do sterownika ************
void mk_ssd1306_display( void ) {

	if( !can_display ) return;

	mk_ssd1306_cmd( 0x21  );			// Command_Column_Address_Set
	mk_ssd1306_cmd( 0x00  );			// Start
	mk_ssd1306_cmd( GLCD_WIDTH - 1  );	// End

	mk_ssd1306_cmd( 0x22  );			// Command_Page_Address_Set
	mk_ssd1306_cmd( 0x00  );			// Start
	mk_ssd1306_cmd( GLCD_HEIGHT - 1 );	// End

	mk_ssd1306_cmd( SSD1306_SETLOWCOLUMN  );
	mk_ssd1306_cmd( SSD1306_SETHIGHCOLUMN );

#if GLCD_HEIGHT == 64
	mk_ssd1306_cmd( 0xB0 );
#endif
#if GLCD_HEIGHT == 32
	mk_ssd1306_cmd( 0xB4 );
#endif


#if USE_I2C == 1
	i2c_write_buf( GLCD_I2C_ADDRESS, 0x40, GLCD_BUF_SIZE, glcd_buf );
#endif

#if USE_I2C == 0

	CS_HI;
	DC_HI;
	CS_LO;

    for (uint16_t i=0; i<GLCD_BUF_SIZE; i++) {
    	spi_xmit_byte( glcd_buf[i] );
    }

    CS_HI;

#endif

}




//************* INICJALIZACJA sterownika SSD1306
void mk_ssd1306_init( void ) {

#if USE_I2C == 1
	i2cSetBitrate( GLCD_I2C_BITRATE );
	twbr_oled = TWBR;
#endif

	_delay_ms(100);

#if USE_I2C == 0

#if USE_RST_PIN == 1
	RST_LO;
	_delay_ms(50);
	RST_HI;
	_delay_ms(50);
#endif

	_delay_ms(10);

	spi_init();

	DC_DDR |= DC;

#endif


    mk_ssd1306_cmd( SSD1306_DISPLAYOFF );
    mk_ssd1306_cmd( SSD1306_SETDISPLAYCLOCKDIV );
    mk_ssd1306_cmd( OLED_CONTRAST );

    mk_ssd1306_cmd( SSD1306_SETDISPLAYOFFSET );
    mk_ssd1306_cmd( 0x0);
    mk_ssd1306_cmd( SSD1306_SETSTARTLINE | 0x0 );
    mk_ssd1306_cmd( SSD1306_CHARGEPUMP );

//    if (vcc == SSD1306_EXTERNALVCC ) mk_ssd1306_cmd( 0x10 );
//    else  mk_ssd1306_cmd( 0x14 );	// SSD1306_SWITCHCAPVCC

    mk_ssd1306_cmd( 0x14 );	// SSD1306_SWITCHCAPVCC

    mk_ssd1306_cmd( SSD1306_MEMORYMODE );
    mk_ssd1306_cmd( 0x00);
    mk_ssd1306_cmd( SSD1306_SEGREMAP | 0x1 );
    mk_ssd1306_cmd( SSD1306_COMSCANDEC );


    mk_ssd1306_cmd( SSD1306_SETCONTRAST );
    mk_ssd1306_cmd( REFRESH_MAX );

//    if (vcc == SSD1306_EXTERNALVCC ) mk_ssd1306_cmd( 0x9F );
//    else mk_ssd1306_cmd( 0xCF );

    mk_ssd1306_cmd( 0xCF );	// SSD1306_SWITCHCAPVCC

    mk_ssd1306_cmd( SSD1306_SETPRECHARGE );

    // ssd1306 - 128 x 32
#ifdef GLCD_RES_128_32
	    mk_ssd1306_cmd( SSD1306_SETMULTIPLEX );
	    mk_ssd1306_cmd( 0x1F );

	    mk_ssd1306_cmd( SSD1306_SETCOMPINS );
	    mk_ssd1306_cmd( 0x02 );
#endif

	// ssd1306 - 128 x 64
#ifdef GLCD_RES_128_64
	    mk_ssd1306_cmd( SSD1306_SETMULTIPLEX );
	    mk_ssd1306_cmd( 0x3F );

	    mk_ssd1306_cmd( SSD1306_SETCOMPINS );
	    mk_ssd1306_cmd( 0x12 );
#endif

	    mk_ssd1306_cmd(SSD1306_SETVCOMDETECT);
	    mk_ssd1306_cmd(0x40);

		mk_ssd1306_cmd( SSD1306_DISPLAYALLON_RESUME );
		mk_ssd1306_cmd( SSD1306_NORMALDISPLAY );

	    mk_ssd1306_cmd( SSD1306_DISPLAYON );

#if SHOW_DEMO_SCREEN == 0
	    glcd_cls();
#endif
	    mk_ssd1306_display();

	    setCurrentFont( &DefaultFont5x8 );

}



void mk_ssd1306_inverse( uint8_t enable ) {
	if( enable ) mk_ssd1306_cmd( SSD1306_INVERTDISPLAY );
	else mk_ssd1306_cmd( SSD1306_NORMALDISPLAY );
}



void mk_ssd1306_flip_vertical( uint8_t flip, uint8_t mirror ) {
	if( !flip ) {
		mk_ssd1306_cmd( 0x80 );		// normal
		mk_ssd1306_cmd( 0xC8 );
		mk_ssd1306_cmd( 0x80 );
		if( !mirror ) mk_ssd1306_cmd( 0xA1 );
		else mk_ssd1306_cmd( 0xA0 );
	} else {
		mk_ssd1306_cmd( 0x80 );		// flipped
		mk_ssd1306_cmd( 0xC0 );
		mk_ssd1306_cmd( 0x80 );
		if( !mirror ) mk_ssd1306_cmd( 0xA0 );
		else mk_ssd1306_cmd( 0xA1 );

	}
}





#endif		// USE_SSD1306
















