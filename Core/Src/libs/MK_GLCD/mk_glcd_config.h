/*                  e-gadget.header
 * mk_glcd_config.h
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



#ifndef MK_GLCD_CONFIG_H_
#define MK_GLCD_CONFIG_H_

//^^^^^^^^^^^^^^^^^^^^^^^ KONFIGURACJA BIBLIOTEKI ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//..... wyb�r chipsetu - rodzaju wy�wietlacza - odkomentuj w�a�ciw� lini� ............................
#define USE_SSD1306						// OLED
//#define USE_SSD1309					// OLED
//#define USE_COG_ST7565R				// COG
//#define USE_NOKIA_PCD8544				// NOKIA
//#define USE_UC1701					// LCX
//#define USE_ST7920					// DIGOLE


//..... wyb�r rozdzielczo�ci - wy�wietlacza - odkomentuj w�a�ciw� lini� ...............................
#define GLCD_RES_128_64						// OLED / COG / LCX / DIGOLE
//#define GLCD_RES_128_32					// OLED
//#define GLCD_RES_84_48					// NOKIA

//..... r�czne ustawienie rozdzielczo�ci - je�li wiesz co robisz ......................................
//..............w przeciwnym wypadku u�yj ustawie� predefiniowanych powy�ej GLCD_RES_xx_xx ............
//..... aby wymusi� r�czne ustawienie rozdzielczo�ci - zakomentuj wszystkie predefiniwane .............
#define GLCD_WIDTH                  128
#define GLCD_HEIGHT                 64



//........ ustawienia pin�w steruj�cych ...............................................................

#define USE_RST_PIN				1			// je�li pin RST sterowany z uC = 1

#define USE_BACKLIGHT			1			// je�li pod�wietlenie sterowane z uC = 1
#define BACKLIGHT_ACTIVE_HIGH	0			// 0-je�li pod�wietlenie za��czane stanem niskim, 1-je�li za��czane stanem wysokim

#define DC 					(1<<2)		//	 <---- A0/DC/RS
#define DC_PORT  			PORTD
#define DC_DDR				DDRD

#if USE_BACKLIGHT == 1
	#define BACKLIGHT_PORT  	PORTD
	#define BACKLIGHT_DDR		DDRD
	#define BACKLIGHT			(1<<7)		//   <---- pod�wietlenie (BackLight)
#endif


#if USE_RST_PIN == 1
	#define RST  				(1<<7)		//	 <---- RST
	#define RST_PORT  			PORTC
	#define RST_DDR				DDRC
#endif




//..... wyb�r magistrali I2C lub SPI dla OLED ................................................
#define USE_I2C					1			// 0-use SPI, 1-use I2C


//................ Ustawienia I2C ............................................................
#define GLCD_I2C_BITRATE		400			// kHz

//..... wyb�r adresu wy�wietlacza na magistrali I2C ..........................................
#define GLCD_I2C_ADDRESS		0x78		// 0x78, 0x7A (dla chipsetu SSD1306)





//..... wy�wietlanie do cel�w testowych przy uruchomieniu ekranu DEMO po inicjalizaji ........
#define SHOW_DEMO_SCREEN		0



#define	FX_MAX_IDX				9




//^^^^^^^^^^^^^^^^^^^^^^^ KONFIGURACJA - KONIEC ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^




#if USE_BACKLIGHT == 1
	#if BACKLIGHT_ACTIVE_HIGH == 1
		#define BACKLIGHT_OFF	BACKLIGHT_PORT &= ~BACKLIGHT
		#define BACKLIGHT_ON	BACKLIGHT_PORT |= BACKLIGHT
	#else
		#ifdef __AVR__
			#define BACKLIGHT_ON	BACKLIGHT_PORT &= ~BACKLIGHT
			#define BACKLIGHT_OFF	BACKLIGHT_PORT |= BACKLIGHT
		#elif defined STM32F3 || defined STM32L4
			#define BACKLIGHT_ON	gpio_pin_LO( BACKLIGHT_PORT, (uint32_t)BACKLIGHT )
			#define BACKLIGHT_OFF	gpio_pin_HI( BACKLIGHT_PORT, (uint32_t)BACKLIGHT )

		#endif
	#endif

#define BACKLIGHT_TOG	BACKLIGHT_PORT ^= BACKLIGHT
#endif

#if USE_RST_PIN == 1
	#define RST_LO 	RST_PORT &= ~RST
	#define RST_HI 	RST_PORT |= RST
#endif

#define DC_LO 	DC_PORT &= ~DC
#define DC_HI 	DC_PORT |= DC

#define CS_LO 	SS_LO
#define CS_HI 	SS_HI


#ifdef GLCD_RES_128_64
	#undef GLCD_WIDTH
	#undef GLCD_HEIGHT
	#define GLCD_WIDTH                  128
	#define GLCD_HEIGHT                 64
#endif

#ifdef GLCD_RES_128_32
	#undef GLCD_WIDTH
	#undef GLCD_HEIGHT
	#define GLCD_WIDTH                  128
	#define GLCD_HEIGHT                 32
#endif

#ifdef GLCD_RES_84_48
	#undef GLCD_WIDTH
	#undef GLCD_HEIGHT
	#define GLCD_WIDTH                  84
	#define GLCD_HEIGHT                 48
#endif



#define GLCD_BUF_SIZE 	( GLCD_WIDTH * GLCD_HEIGHT / 8 )

#endif /* MK_GLCD_CONFIG_H_ */
















