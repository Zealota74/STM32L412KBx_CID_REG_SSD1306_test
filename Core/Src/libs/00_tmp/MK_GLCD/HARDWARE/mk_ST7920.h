/*                  e-gadget.header
 * mk_ST7920.h
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

#ifndef MK_GLCD_HARDWARE_MK_ST7920_H_
#define MK_GLCD_HARDWARE_MK_ST7920_H_


/* Basic ST7920 Commands */
#define ST7920_CMD_CLEAR 			0x01 // Clear Display
#define ST7920_CMD_HOME 			0x02 // Move Cursor Home
#define ST7920_CMD_EM 				0x04 // Entry Mode Base
#define ST7920_CMD_EM_INCRR 		0x02 // Increment Cursor Right
#define ST7920_CMD_EM_INCRL 		0x00 // Increment Cursor Left
#define ST7920_CMD_EM_SHFTR 		0x03 // Shift Display Right
#define ST7920_CMD_EM_SHFTL 		0x01 // Shift Display Left
#define ST7920_CMD_DC 				0x08 // Display Control
#define ST7920_CMD_DC_DISPON 		0x04 // Display On
#define ST7920_CMD_DC_CURON 		0x02 // Cursor On
#define ST7920_CMD_DC_BLNKON 		0x01 // Blink On
#define ST7920_CMD_FNC 				0x20 // Function Set
#define ST7920_CMD_FNC_DL8 			0x10 // 8-Bit Interface
#define ST7920_CMD_FNC_DL4 			0x00 // 4-Bit Interface
#define ST7920_CMD_FNC_EXTINS 		0x04 // Extended Instruction Set
#define ST7920_CMD_FNC_BASINS 		0x00 // Basic Instruction Set
#define ST7920_CMD_CGRAM_ADDR 		0x40 // Set CGRAM Address
#define ST7920_CMD_DDRAM_ADDR 		0x80 // Set DDRAM Address

#define	FUNC_SET_BAS	( ST7920_CMD_FNC | ST7920_CMD_FNC_DL8 | ST7920_CMD_FNC_BASINS )
#define FUNC_SET_EXT	( ST7920_CMD_FNC | ST7920_CMD_FNC_DL8 |	ST7920_CMD_FNC_EXTINS )
#define DISPLAY_CMD		( ST7920_CMD_DC | ST7920_CMD_DC_DISPON )
#define ENTRY_MODE_SET	( ST7920_CMD_EM | ST7920_CMD_EM_INCRR)


#endif /* MK_GLCD_HARDWARE_MK_ST7920_H_ */








