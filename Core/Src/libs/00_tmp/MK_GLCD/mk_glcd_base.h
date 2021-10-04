/*                  e-gadget.header
 * mk_glcd_base.h
 *
 *  Created on: 2019-05-21
 *    Modyfied: 2019-06-19 09:28:21
 *      Author: Miros³aw Kardaœ
 *         web: www.atnel.pl
 *
 *	Library: MK GLCD  ver: 1.1a
 *
 * 	 Obs³ugiwane wyœwietlacze/chipsety/magistrale:
 * 
 * 	 1. OLED 128x64 - SSD1306 I2C/SPI
 * 	 2. OLED 128x32 - SSD1306 I2C/SPI
 * 	 3. COG 128x64  - ST7565R SPI
 * 	 4. NOKIA 84x48 - PCD8544 SPI
 * 	 5. COG 128x64  - UC1701x SPI
 * 
 * 	 Biblioteka "MK GLCD" wymaga do prawid³owej pracy
 * 	 jednej z dwóch bibliotek:
 * 
 * 	 1. MK_I2C		https://sklep.atnel.pl/pl/p/0581_0582-MK-I2C-AVR-Biblioteka-C/244
 * 	 2. MK_SPI		https://sklep.atnel.pl/pl/p/0567_0570-MK-SPI-AVR-Biblioteka-C-/241
 * 
 * 	 F_CPU: 1MHz - 24MHz
 * 	 MCU: all ATmega uC with minimum 2Kb RAM
 * 	
 * 	 Uwaga! przed kompilacj¹ nale¿y dodaæ opcjê w ustawieniach toolchaina
 * 	 C/C++ Build/Settings/AVR C Linker/General/Other Arguments
 * 	 -Wl,-gc-sections
 * 	
 *
 *
 */
#include <string.h>

#include "COMMON/mk_glcd_common.h"

#include "mk_glcd_config.h"
#include "GRAPHICS/mk_glcd_graphics.h"

#include "TEXT/mk_glcd_text.h"
#include "TEXT/mk_glcd_scroll_text.h"

#include "OBJECTS/mk_analog_meter.h"
#include "OBJECTS/mk_bar_graph.h"
#include "OBJECTS/aclock.h"
#include "SCREEN_SAVER/mk_scr_saver.h"



#ifndef MK_GLCD_BASE_H_
#define MK_GLCD_BASE_H_

typedef struct {
	uint8_t active;
	uint8_t left;
	uint8_t right;
	uint8_t top;
	uint8_t bottom;
} TVIEWPORT;

extern uint8_t glcd_buf[];

extern int cur_x;
extern int cur_y;

extern uint8_t szachownica;
extern int cur_x, cur_y;
extern uint8_t can_display;

extern TVIEWPORT viewport;





inline void __attribute__((always_inline)) glcd_cls( void ) {
	memset( glcd_buf, 0x00, GLCD_BUF_SIZE );
	cur_x = 0;
	cur_y = 0;
	can_display = 1;
}


inline void __attribute__((always_inline)) glcd_backlight( uint8_t enabled  ) {
#if USE_BACKLIGHT == 1
		if( enabled ) BACKLIGHT_ON;
		else BACKLIGHT_OFF;
#endif
}


#ifdef USE_SSD1306

#define mk_ssd1306_init(...)				glcd_init( __VA_ARGS__ )
#define mk_ssd1306_setPixel(...)			glcd_setPixel( __VA_ARGS__ )
#define mk_ssd1306_getPixel(...)			glcd_getPixel( __VA_ARGS__ )
#define mk_ssd1306_display(...)				glcd_display( __VA_ARGS__ )
#define mk_ssd1306_flip_vertical(...)		glcd_flip_vertical( __VA_ARGS__ )
#define mk_ssd1306_inverse(...)				glcd_inverse( __VA_ARGS__ )

#endif


#ifdef USE_SSD1309

#define mk_ssd1309_init(...)				glcd_init( __VA_ARGS__ )
#define mk_ssd1309_setPixel(...)			glcd_setPixel( __VA_ARGS__ )
#define mk_ssd1309_getPixel(...)			glcd_getPixel( __VA_ARGS__ )
#define mk_ssd1309_display(...)				glcd_display( __VA_ARGS__ )
#define mk_ssd1309_flip_vertical(...)		glcd_flip_vertical( __VA_ARGS__ )
#define mk_ssd1309_inverse(...)				glcd_inverse( __VA_ARGS__ )

#endif


#ifdef USE_NOKIA_PCD8544

#define mk_pcd8544_init( ... )				glcd_init( __VA_ARGS__ )
#define mk_pcd8544_setPixel(...)			glcd_setPixel( __VA_ARGS__ )
#define mk_pcd8544_getPixel(...)			glcd_getPixel( __VA_ARGS__ )
#define mk_pcd8544_display(...)				glcd_display( __VA_ARGS__ )
#define mk_pcd8544_flip_vertical(...)		glcd_flip_vertical( __VA_ARGS__ )
#define mk_pcd8544_inverse(...)				glcd_inverse( __VA_ARGS__ )

#endif


#ifdef USE_COG_ST7565R

#define mk_st7565_init( ... )				glcd_init( __VA_ARGS__ )
#define mk_st7565_setPixel(...)				glcd_setPixel( __VA_ARGS__ )
#define mk_st7565_getPixel(...)			glcd_getPixel( __VA_ARGS__ )
#define mk_st7565_display(...)				glcd_display( __VA_ARGS__ )
#define mk_st7565_flip_vertical(...)		glcd_flip_vertical( __VA_ARGS__ )
#define mk_st7565_inverse(...)				glcd_inverse( __VA_ARGS__ )

#endif


#ifdef USE_UC1701

#define mk_uc1701_init(...)					glcd_init( __VA_ARGS__ )
#define mk_uc1701_setPixel(...)				glcd_setPixel( __VA_ARGS__ )
#define mk_uc1701_getPixel(...)			glcd_getPixel( __VA_ARGS__ )
#define mk_uc1701_display(...)				glcd_display( __VA_ARGS__ )
#define mk_uc1701_flip_vertical(...)		glcd_flip_vertical( __VA_ARGS__ )
#define mk_uc1701_inverse(...)				glcd_inverse( __VA_ARGS__ )


#endif


#ifdef USE_ST7920

#define mk_st7920_init(...)					glcd_init( __VA_ARGS__ )
#define mk_st7920_setPixel(...)				glcd_setPixel( __VA_ARGS__ )
#define mk_st7920_getPixel(...)			glcd_getPixel( __VA_ARGS__ )
#define mk_st7920_display(...)				glcd_display( __VA_ARGS__ )
#define mk_st7920_flip_vertical(...)		glcd_flip_vertical( __VA_ARGS__ )
#define mk_st7920_inverse(...)				glcd_inverse( __VA_ARGS__ )

#endif



#endif /* MK_GLCD_BASE_H_ */

















