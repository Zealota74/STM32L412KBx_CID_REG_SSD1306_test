/*
 * sw_ssd1306.h
 *
 *  Created on: 19 paź 2015
 *      Author: simon
 */

#ifndef LIBS_SW_SSD1306_H_
#define LIBS_SW_SSD1306_H_

#ifdef __AVR__
#include "../sw_transmission.h"
#else
#define HARD_I2C	1
#define TRANSMISSION_MODE	HARD_I2C
#endif

typedef enum { page0, page1, page2, page3, page4, page5, page6, page7} TPAGE;

// Rozdzielczość wyświetlacza
#define SSD1306_128_64
//#define SSD1306_128_32
#define BLACK	0
#define WHITE	1

#define SSD1306_WIDTH		128

#if (TRANSMISSION_MODE==SOFT_I2C)||(TRANSMISSION_MODE==HARD_I2C)
	#define I2C_ADDRESS			0x78
	#define I2C_ADDRESS_BACK	0x7A // zapasowy adres
	#define I2C_CTRL_DC_0		0x00 //
	#define I2C_CTRL_DC_1		0x40 // 01000000b
#endif

#if defined SSD1306_128_64
	#define SSD1306_HEIGHT	64
#endif
#if defined SSD1306_128_32
	#define SSD1306_HEIGHT	32
#endif

#define REFRESH_MIN 0x80
#define REFRESH_MID 0xB0
#define REFRESH_MAX 0xF0


#define SSD1306_SETCONTRAST			0x81
#define SSD1306_DISPLAYALLON_RESUME	0xA4
#define SSD1306_DISPLAYALLON		0xA5
#define SSD1306_NORMALDISPLAY		0xA6
#define SSD1306_INVERTDISPLAY		0xA7
#define SSD1306_DISPLAYOFF			0xAE
#define SSD1306_DISPLAYON			0xAF

#define SSD1306_SETDISPLAYOFFSET	0xD3
#define SSD1306_SETCOMPINS			0xDA

#define SSD1306_SETVCOMDETECT		0xDB

#define SSD1306_SETDISPLAYCLOCKDIV	0xD5
#define SSD1306_SETPRECHARGE		0xD9

#define SSD1306_SETMULTIPLEX		0xA8

#define SSD1306_SETLOWCOLUMN		0x00
#define SSD1306_SETHIGHCOLUMN		0x10	//16d
#define SSD1306_SETSTARTLINE		0x40	//64d

#define SSD1306_MEMORYMODE			0x20
#define SSD1306_COLUMNADDR			0x21
#define SSD1306_PAGEADDR			0x22
#define SSD1306_PAGESTARTADDR		0xB0

#define SSD1306_COMSCANINC			0xC0
#define SSD1306_COMSCANDEC			0xC8

//#define SSD1306_SEGMAP			0xA0
#define SSD1306_SEGREMAP			0xA0

#define SSD1306_CHARGEPUMP			0x8D

#define SSD1306_EXTERNALVCC			0x1
#define SSD1306_SWITCHCAPVCC		0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL							0x2F
#define SSD1306_DEACTIVATE_SCROLL						0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA				0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL					0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL					0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL	0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL		0x2A

void sw_ssd1306_init( uint8_t vcc, uint8_t refresh );
void sw_ssd1306_ram_to_display( uint8_t shift );
void sw_ssd1306_set_brightness( uint8_t bright );
void sw_ssd1306_display_OnOff( uint8_t onoff );


#endif /* LIBS_SW_SSD1306_H_ */
