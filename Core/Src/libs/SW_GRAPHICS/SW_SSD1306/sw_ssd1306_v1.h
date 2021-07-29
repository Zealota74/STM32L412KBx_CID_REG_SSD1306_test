/*
* rgb_matrix_64x32.h
 *
 *  Created on: 11.04.2017
 *      Author: simon
 */

#ifndef LIBS_RGB_MATRIX_64X32_H_
#define LIBS_RGB_MATRIX_64X32_H_

#include "sw_ssd1306_pin_conf.h"

#ifdef __AVR__
#include "../sw_transmission.h"
#else
#define HARD_I2C	1
#define TRANSMISSION_MODE	HARD_I2C
#endif

#if (TRANSMISSION_MODE==SOFT_I2C)||(TRANSMISSION_MODE==HARD_I2C)
	#define I2C_ADDRESS			0x78
	#define I2C_ADDRESS_BACK	0x7A // zapasowy adres
	#define I2C_CTRL_DC_0		0x00 //
	#define I2C_CTRL_DC_1		0x40 // 01000000b
#endif

// Rozdzielczość wyświetlacza
#define SSD1306_128_64
//#define SSD1306_128_32
#define BLACK	0
#define WHITE	1

#define SSD1306_WIDTH	128
#define SSD1306_HEIGHT	64

#ifdef SSD1306_SPI
#define CS_LO	gpio_pin_LO( SPI_NSS_PORT, SPI_NSS_PIN )
#define CS_HI	gpio_pin_HI( SPI_NSS_PORT, SPI_NSS_PIN )
#define SCK_LO	gpio_pin_LO( SPI_SCK_PORT, SPI_SCK_PIN )
#define SCK_HI	gpio_pin_HI( SPI_SCK_PORT, SPI_SCK_PIN )
#define DC_LO	gpio_pin_LO( SPI_DC_PORT, SPI_DC_PIN )
#define DC_HI	gpio_pin_HI( SPI_DC_PORT, SPI_DC_PIN )
#define MOSI_LO	gpio_pin_LO( SPI_MOSI_PORT, SPI_MOSI_PIN )
#define MOSI_HI	gpio_pin_HI( SPI_MOSI_PORT, SPI_MOSI_PIN )
#define RES_LO	gpio_pin_LO( RES_PORT, RES_PIN )
#define RES_HI	gpio_pin_HI( RES_PORT, RES_PIN )
#endif

#define REFRESH_MIN 0x80
#define REFRESH_MID 0xB0
#define REFRESH_MAX 0xF0

typedef enum {page0, page1, page2, page3, page4, page5, page6, page7} TPAGE;

#define OLED_CONTRAST 		240
#define GLCD_RES_128_64

#define SSD1306_SETCONTRAST			0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
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
#define SSD1306_SETHIGHCOLUMN		0x10

#define SSD1306_SETSTARTLINE		0x40

#define SSD1306_MEMORYMODE			0x20

#define SSD1306_COMSCANINC			0xC0
#define SSD1306_COMSCANDEC			0xC8

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


#define DRIVER_MODE_1		1
#define DRIVER_MODE_2		2

extern uint8_t *set_get_bright ( uint16_t bright );
extern uint8_t *set_get_bright2( uint8_t cnt );
extern uint8_t *set_driver_mode( void );

extern void set_framerate( uint16_t framerateLOC );

extern void panel_HW_start( void );

extern void sw_ssd1306_init ( void );
extern void sw_RGB_panel_deinit( void );

extern void DMA1_Channel4_IRQHandler(void);
extern void DMA1_Channel5_IRQHandler(void);
extern void DMA2_Channel3_IRQHandler(void);
extern void TIM1_UP_TIM16_IRQHandler(void);

extern CCMRAM void graphic_set_pixel_RAM( volatile T_DISPLAY *buffer, int16_t x, int16_t y, const T_COLOR *color );


#ifdef SOFTWARE_CLK_GEN
void TIM3_IRQHandler( void );
#endif

extern void sw_heartbeat(void);
extern void sw_display_onOff(void);

extern void register_scroll_event_callback( void (*callback)( volatile T_DISPLAY * buffer, uint32_t color, int8_t direction ) );
extern void SW_SCROLL_EVENT( volatile T_DISPLAY * buffer, uint32_t color, int8_t direction );


extern void sw_ssd1306_display( void );
extern void sw_display_cls( uint8_t *buffer, uint8_t pattern );
volatile uint8_t scrollTrigs;

#endif /* LIBS_RGB_MATRIX_64X32_H_ */
