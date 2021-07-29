/*                  e-gadget.header
 * mk_SSD1309.h
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

#ifndef MK_GLCD_MK_SSD1309_H_
#define MK_GLCD_MK_SSD1309_H_


#ifdef USE_SSD1309


/*** UWAGA! ... konfiguracji prêdkoœci I2C - Bitrate
 * 				nale¿y dokonaæ w bibliotece MK_I2C w pliku "mk_i2c.h"
 * 				max Bitrate = ~600 kHz
 */


#define OLED_CONTRAST		0xB0


//----------------------------------------------- koniec konfiguracji -------------------


#define REFRESH_MIN 0x80
#define REFRESH_MID 0xB0
#define REFRESH_MAX 0xF0

typedef enum {page0, page1, page2, page3, page4, page5, page6, page7} TPAGE;


#define SSD1309_SETCONTRAST 0x81
#define SSD1309_DISPLAYALLON_RESUME 0xA4
#define SSD1309_DISPLAYALLON 0xA5
#define SSD1309_NORMALDISPLAY 0xA6
#define SSD1309_INVERTDISPLAY 0xA7
#define SSD1309_DISPLAYOFF 0xAE
#define SSD1309_DISPLAYON 0xAF

#define SSD1309_SETDISPLAYOFFSET 0xD3
#define SSD1309_SETCOMPINS 0xDA

#define SSD1309_SETVCOMDETECT 0xDB

#define SSD1309_SETDISPLAYCLOCKDIV 0xD5
#define SSD1309_SETPRECHARGE 0xD9

#define SSD1309_SETMULTIPLEX 0xA8

#define SSD1309_SETLOWCOLUMN 0x00
#define SSD1309_SETHIGHCOLUMN 0x10

#define SSD1309_SETSTARTLINE 0x40

#define SSD1309_MEMORYMODE 0x20

#define SSD1309_COMSCANINC 0xC0
#define SSD1309_COMSCANDEC 0xC8

#define SSD1309_SEGREMAP 0xA0

#define SSD1309_CHARGEPUMP 0x8D

#define SSD1309_EXTERNALVCC 0x1
#define SSD1309_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1309_ACTIVATE_SCROLL 0x2F
#define SSD1309_DEACTIVATE_SCROLL 0x2E
#define SSD1309_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1309_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1309_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1309_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1309_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A




#endif //  USE_SSD1309


#endif /* MK_GLCD_MK_SSD1309_H_ */
















