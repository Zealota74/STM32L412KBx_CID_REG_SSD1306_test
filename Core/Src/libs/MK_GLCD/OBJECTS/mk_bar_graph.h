/*                  e-gadget.header
 * mk_bar_graph.h
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

#ifndef MK_GLCD_OBJECTS_MK_BAR_GRAPH_H_
#define MK_GLCD_OBJECTS_MK_BAR_GRAPH_H_


typedef struct {
	uint8_t		checker: 1;
	uint8_t		outline_checker: 1;
	uint8_t 	inspace: 1;
	uint8_t 	up_ticks: 1;
	uint8_t 	down_ticks: 1;
	uint8_t		values_visible:1;
	uint8_t		values_down: 1;
	uint8_t 	vertical:1;
	uint8_t 	show_text:1;
	uint8_t 	tick_length:4;
} TBARPROPERTIES;

extern TBARPROPERTIES	bar_properties;









void glcd_drawbar( int x, int y, int width, int height, int8_t percent );



#endif /* MK_GLCD_OBJECTS_MK_BAR_GRAPH_H_ */








