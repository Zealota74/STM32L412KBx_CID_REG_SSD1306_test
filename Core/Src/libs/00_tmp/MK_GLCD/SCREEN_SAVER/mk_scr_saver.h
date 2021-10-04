/*                  e-gadget.header
 * mk_scr_saver.h
 *
 *  Created on: 2019-06-15
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

#ifndef MK_GLCD_SCREEN_SAVER_MK_SCR_SAVER_H_
#define MK_GLCD_SCREEN_SAVER_MK_SCR_SAVER_H_



typedef struct {
	int8_t x;
	int8_t lx;
	int8_t y;
	int8_t ly;
	uint8_t r;
	int8_t xoffset;
	int8_t yoffset;

} TBUBLE;



extern volatile uint8_t screen_saver_emergency_exit;



/* 	W celu uruchomienia Screen Savera nale¿y bezwzglêdnie przekazaæ:
 *
 * 		1. *scr_tmr - wskaŸnik do wolnego timera programowego
 * 		2. break_callback - wskaŸnik do funkcji przerywaj¹cej ScrenSaver
 *
 * 		poza tym mo¿na ale nie jest to wymagane, przekazaæ tak¿e wskaŸnik do funkcji, która ma siê wykonaæ
 * 		po zakoñczeniu pracy ScreenSavera
 *
 * 		2. onexit_callback
 *
 * 		Uwaga! zakoñczenie pracy Screen Savera nast¹pi gdy funkcja zwrotna break_callback przeka¿e jako rezultat
 * 		wartoœæ ró¿n¹ od zera. Funkcja ta mo¿e s³u¿yæ tak¿e do realizacji w tle pracy ScreenSavera innych krytycznie
 * 		wa¿nych procesów jak np zdarzenie obs³ugi UART, I2C, SPI czy podczerwieni lub klawiatury i innych.
 *
 */
void register_scr_saver( volatile void * scr_tmr, uint8_t (*break_callback)(void), void (*onexit_callback)(void) );


// pierwszy Screen SAVER - B¥BELKI
// iloœæ b¹belków buubles_cnt - musi byæ wiêksza od zera.
// Uwaga! Im wiêcej zdeklarowanych b¹belków tym wiêcej pamiêci RAM potrzeba do pracy funkcji.
// zalecane wartoœci od 3 do 10 - mo¿na próbowaæ wiêcej ;) do zawieszenia pracy lub nieoczekiwanego zachowania mikrokontrolera z powodu przepe³nienia stosu
void glcd_screen_saver1( uint8_t buubles_cnt );

#endif /* MK_GLCD_SCREEN_SAVER_MK_SCR_SAVER_H_ */








