/*
 * sw_ssd1306.c
 *
 *  Created on: 19 paź 2015
 *      Author: simon
 */
#ifdef __AVR__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#else
#include "sw_mcu_conf.h"
#include "../SW_BOARD/sw_gpio.h"
#include "../SW_TIMERS/sw_soft_timers.h"
#include "../sw_i2c_simple_v2.h"
#define PROGMEM
#define sw_hardware_TWI_write_buf	sw_i2c_write_block
#define _delay_ms	delay_ms
#endif
#include <string.h>
#include <stdlib.h>

#include "sw_ssd1306.h"
#include "bitmaps.h"

/***************** Wysyła KOMENDĘ do wyświetlacza *******************/
void sw_ssd1306_cmd(uint8_t cmd) {
#if TRANSMISSION_MODE==SOFT_SPI
	#if USE_CS==1
	CS_HI;
	#endif
	DC_LO;
	#if USE_CS==1
	CS_LO;
	#endif
//	sw_software_SPI_write( cmd );
	sw_transmision_write( cmd );
	#if USE_CS==1
	CS_HI;
	#endif
#elif (TRANSMISSION_MODE==SOFT_I2C) || (TRANSMISSION_MODE==HARD_I2C)
//	sw_hardware_TWI_write_buf(I2C_ADDRESS, I2C_CTRL_DC_0, 1, &cmd);
	sw_i2c_write_reg( I2C_ADDRESS, I2C_CTRL_DC_0, cmd );
#endif
	delay_us(100);
}
/*********************************************************************/

/****************** Wysyła DANE do wyświetlacza **********************/
void sw_ssd1306_data(uint8_t data) {
#if TRANSMISSION_MODE==SOFT_SPI

	#if USE_CS==1
	CS_HI;
	#endif
	DC_HI;
	#if USE_CS==1
	CS_LO;
	#endif
//	sw_software_SPI_write( data );
	sw_transmision_write( data );
	#if USE_CS==1
	CS_HI;
	#endif
#elif (TRANSMISSION_MODE==SOFT_I2C) || (TRANSMISSION_MODE==HARD_I2C)
	sw_i2c_write_reg( I2C_ADDRESS, I2C_CTRL_DC_1, data );
#endif
	delay_us(100);
}
/*****************************************************************/

/******* Inicjalizacja kontrolera SSD1306 oraz peryferii *********/
void sw_ssd1306_init(uint8_t vcc, uint8_t refresh) {
	_delay_ms(500);
#if TRANSMISSION_MODE==SOFT_SPI
	sw_software_SPI_init();

	#if USE_CS==1
	CS_HI;
	_delay_ms(25);
	CS_LO;
	_delay_ms(25);
	CS_HI;
	#endif

	#if USE_RST==1
	RST_HI;
	_delay_ms(25);
	RST_LO;
	_delay_ms(25);
	RST_HI;
	#endif
#elif (TRANSMISSION_MODE==HARD_I2C)||(TRANSMISSION_MODE==SOFT_I2C)
	sw_i2c_simple_init();
//	_delay_ms(50); // Sprawdzić po co jest ta zwłoka, może do SOFT_I2C
#endif
/**/

// Set entire DISPLAY OFF
	sw_ssd1306_cmd(SSD1306_DISPLAYOFF);						// 0xAE

// Page addressing mode
	sw_ssd1306_cmd(SSD1306_SETLOWCOLUMN);
	sw_ssd1306_cmd(SSD1306_SETHIGHCOLUMN);

// Set display STARTLINE
	sw_ssd1306_cmd(SSD1306_SETSTARTLINE | 0);				// line #0

// Set CONTRAST
	sw_ssd1306_cmd(SSD1306_SETCONTRAST);					// 0x81
	sw_ssd1306_cmd(255);									// Brigthness to maximum

// Set segment REMAP
	sw_ssd1306_cmd(SSD1306_SEGREMAP | 0x01);

// Set COM output scan direction
	sw_ssd1306_cmd(SSD1306_COMSCANDEC);						// 0xc8, Set COM/Row Scan Direction

// set NORMAL DISPLAY
	sw_ssd1306_cmd( SSD1306_NORMALDISPLAY );
//	sw_ssd1306_cmd( SSD1306_INVERTDISPLAY );

// Set MUX ratio
	sw_ssd1306_cmd(SSD1306_SETMULTIPLEX);					// 0xA8
	sw_ssd1306_cmd(0x3F);
// Set COM PINS hardware configuration
	sw_ssd1306_cmd(SSD1306_SETCOMPINS);						// 0xDA
	sw_ssd1306_cmd(0x12);

// Set display OFFSET
	sw_ssd1306_cmd(SSD1306_SETDISPLAYOFFSET);				// 0xD3
	sw_ssd1306_cmd(0x00);

// Set Pre-charge period
	sw_ssd1306_cmd(SSD1306_SETPRECHARGE);					// 0xd9
	sw_ssd1306_cmd(0xf1);									//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock

// Set VCOMH deselect level
 	sw_ssd1306_cmd( SSD1306_SETVCOMDETECT );
	sw_ssd1306_cmd( 0x40);

// MEMORY MODE set
    sw_ssd1306_cmd(SSD1306_MEMORYMODE);					// 0x20
    sw_ssd1306_cmd(0x10);								// 0x10: PA mode, 0x00: H mode 0x0 act like ks0108

    sw_ssd1306_cmd(SSD1306_COLUMNADDR );				// 0x21
	sw_ssd1306_cmd( 0 );
	sw_ssd1306_cmd( SSD1306_WIDTH - 1 );				// 128 - 1 = 127

	sw_ssd1306_cmd(SSD1306_PAGEADDR);					// 0x22
	sw_ssd1306_cmd(page0);
	sw_ssd1306_cmd(page7);

// Set OSC Frequency
	sw_ssd1306_cmd(SSD1306_SETDISPLAYCLOCKDIV);        // 0xD5
	sw_ssd1306_cmd( refresh );                         // the suggested ratio 0x80

// Set CHARGE PUMP regulator
	sw_ssd1306_cmd(SSD1306_CHARGEPUMP);                // 0x8D
	sw_ssd1306_cmd(0x14);							   //--set(0x10) disable

#if defined SSD1306_128_32
  sw_ssd1306_cmd( SSD1306_SETMULTIPLEX );
  sw_ssd1306_cmd( 0x1F );
  sw_ssd1306_cmd( SSD1306_SETCOMPINS );
  sw_ssd1306_cmd( 0x02 );
#endif

#if defined SSD1306_128_64
  sw_ssd1306_cmd(SSD1306_SETMULTIPLEX);
  sw_ssd1306_cmd(0x3F);
  sw_ssd1306_cmd(SSD1306_SETCOMPINS);
  sw_ssd1306_cmd(0x12);
#endif

	sw_ssd1306_cmd( SSD1306_DISPLAYALLON_RESUME );
	sw_ssd1306_cmd( SSD1306_DISPLAYON);
}
/*****************************************************************/

/*************** Zapisuje bufor do wyświetlacza ******************
 * 16MHz, hard I2C, 600kHz: 13.5 ms
 */
void sw_ssd1306_ram_to_display( uint8_t shift ) {
	sw_ssd1306_cmd( 0x21  );				// Command_Column_Address_Set
	sw_ssd1306_cmd( 0x00  );				// Start
	sw_ssd1306_cmd( SSD1306_WIDTH - 1  );	// End

	sw_ssd1306_cmd( 0x22  );				// Command_Page_Address_Set
	sw_ssd1306_cmd( 0x00  );				// Start
	sw_ssd1306_cmd( SSD1306_WIDTH - 1 );	// End

	sw_ssd1306_cmd( SSD1306_SETLOWCOLUMN  );
	sw_ssd1306_cmd( SSD1306_SETHIGHCOLUMN );

#if SSD1306_HEIGHT == 64
	sw_ssd1306_cmd( 0xB0 );
#endif
#if SSD1306_HEIGHT == 32
	sw_ssd1306_cmd( 0xB4 );
#endif

#if TRANSMISSION_MODE==SOFT_SPI
	#if USE_CS==1
	CS_HI;
	#endif
	DC_HI;
	#if USE_CS==1
	CS_LO;
	#endif

	uint16_t i;
	for ( i=0; i<(SSD1306_WIDTH*SSD1306_HEIGHT)/8; i++) {
		sw_software_SPI_write( mem_128x64_buf[i] );
	}
	#if USE_CS==1
	CS_HI;
	#endif
#elif TRANSMISSION_MODE==HARD_I2C
//	for ( uint16_t i=0; i<1024; i++ ){
//		mem_128x64_buf[i] = i%255;
//	}
	sw_hardware_TWI_write_buf( I2C_ADDRESS, I2C_CTRL_DC_1, sizeof(mem_128x64_buf), mem_128x64_buf );
#endif
}
/*******************************************************************/

/************************** Ustawia jasnosc ************************/
void sw_ssd1306_set_brightness( uint8_t bright ) {
	sw_ssd1306_cmd( SSD1306_SETCONTRAST );
	sw_ssd1306_cmd( bright );
}
/*******************************************************************/

void sw_ssd1306_display_OnOff( uint8_t onoff ) {
	if ( onoff == SSD1306_DISPLAYON ) {
		sw_ssd1306_cmd( SSD1306_DISPLAYALLON_RESUME );
		sw_ssd1306_cmd( SSD1306_DISPLAYON );						// 0xAE
	} else
		sw_ssd1306_cmd( SSD1306_DISPLAYOFF );						// 0xAE
}
