/*                  e-gadget.header
 * mk_analog_meter.h
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

#ifndef MK_GLCD_MK_ANALOG_METER_H_
#define MK_GLCD_MK_ANALOG_METER_H_


//*************************** konfiguracja pozycji i skali wska�nika analogowego ************************************

#define USE_DEFAULT_PERCENT_CALLBACK		1

// dla rozdzielczo�ci 128 x 64 - pozycja wy�rodkowana
#define ASCALE		100
#define XOFFSET		64
#define YOFFSET		32


// dla NOKIA dla rozdzielczo�ci 84 x 48 - pozycja wy�rodkowana
//#define ASCALE		70
//#define XOFFSET		42
//#define YOFFSET		2

//****************************************** koniec konfiguracji pozycjonowania wska�nika analogowego ***************


typedef struct {
	int8_t scale;
	int8_t xoffset;
	int8_t yoffset;
	uint8_t needlen;
	int min;
	int max;
	int amax;
	int value;
	int sval;
	uint8_t fract_pos;
	uint8_t delay;
	uint8_t speed;
	int8_t off_range;
	int8_t pval;
	int8_t last_pval;
	uint8_t direct_draw;
	void (*needle_callback)(void);
	const FONT_INFO * font;
	uint8_t dotted_needle;
} TANALOGMETER_CONFIG;

extern TANALOGMETER_CONFIG ameter;

inline void ameter_set_scale( uint8_t sc ) {
	ameter.scale = sc-38;
}

inline void ameter_set_xoffset( uint8_t xo ) {
	ameter.xoffset = xo-2;
}

inline void ameter_set_yoffset( uint8_t yo ) {
	ameter.yoffset = yo+50;
}

inline void ameter_set_needlen( void ) {
	ameter.needlen = ameter.scale-4;
}


#define SCALE		(ASCALE-38)

#define OFX			(XOFFSET-2)
#define OFY 		(YOFFSET+50)

#define NEEDOFSET	0//20

#define NEEDLEN		(SCALE-4) //  55


void ameter_init( int min, int max, uint8_t fract_pos, const FONT_INFO * font, uint8_t speed, uint8_t delay, int8_t off_range, void (*needle_callback)(void) );

void AMETER_EVENT( void );

void ameter_set_val( int val );


void draw_analog_meter( void );
void draw_Needle( uint8_t need_solid, uint8_t need_change_val );

#if defined STM32F3 || defined STM32L4
extern const char * PSTR(const char * stringPTR);
#endif

#endif /* MK_GLCD_MK_ANALOG_METER_H_ */














