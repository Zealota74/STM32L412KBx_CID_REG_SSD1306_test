/*                  e-gadget.header
 * mk_glcd_scroll_text.h
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

#ifndef MK_GLCD_TEXT_MK_GLCD_SCROLL_TEXT_H_
#define MK_GLCD_TEXT_MK_GLCD_SCROLL_TEXT_H_


#define FX_COUNTER1_LEN_MAX			10
#define FX_COUNTER2_LEN_MAX			4


enum {
	fx_not_active,
	fx_ld_up, fx_ld_down,
	fx_left, fx_right,
	fx_letters_T2Bin, fx_letters_T2Bout,
	fx_left_letters, fx_right_letters,
	fx_left_out, fx_right_out,															// change text allowed
	fx_left_sin, fx_right_sin, 															// change text allowed
	fx_left_sin_letters0, fx_left_sin_letters1, fx_left_sin_letters2,					// change text allowed
};


typedef struct {
	uint8_t 		fx_type;
	int 			y;
	uint8_t 		font_height;
	char 			* txt;
	char 			* txtA;
	const char 		* txtP;
	int 			txt_len;
	int 			left_x;
	int 			right_x;
	int 			top_y;
	int 			bot_y;
	uint8_t 		count;
	const FONT_INFO * font;
	int 			txt_x;
	int 			txt_y;

	int 			start_x;
	int 			end_x;
	uint16_t 		delay;
	uint8_t 		xp;
	uint8_t 		alidx;
	char 			letter[2];
	uint8_t 		ram;
	uint8_t 		znacznik1;
	uint8_t 		invert_background;
} TMKFX;


typedef struct {
	uint8_t 			len1;				// ilo�� znak�w dla counter1 (przed przecinkiem)
	uint8_t 			len2;				// ilo�� znak�w dla counter2 (po przecinku)
	uint8_t 			interspace_px1;		// interspace_px1 - odst�py mi�dzy znakami dla counter1
	uint8_t 			interspace_px2;		// interspace_px2 - odst�py mi�dzy znakami dla counter2
	const FONT_INFO 	* font1;	// font1 - font dla counter1.
	const FONT_INFO 	* font2;	// font2 - font dla counter2. Je�li ma by� niewidoczny counter2 to font2 = 0
	char 				atr1;					// atr1 - atrybut 'i', 'f' lub ' ' dla counter1
	char 				atr2;					// atr2 - atrybut 'i', 'f' lub ' ' dla counter2
	uint8_t 			align;				// wyr�wnanie licznika: _left, _center, _right
	uint8_t 			leading_zero;		// 0 - brak zer wiod�cych, 1 - maj� by� wy�wietlane zera wiod�ce - wtedy znak minus je�li jest to na pocz�tku
	uint8_t 			as_clock;			// opcja dla zegarka
	char				clock_separator;	// separator dla zegarka - domy�lnie dwukropek
	uint8_t 			speed;				// opcje pr�dko�ci przewijania 1-minimum
	uint8_t 			delay;				// opcje op�nienia przewijania 1-minimum
} TCOUNTERPARAMSINIT;








void fx_change_text( uint8_t idx, char * txt );
void fx_stop_now( uint8_t idx );

void fx_init( uint8_t idx, int y, int left_x, int right_x, char * txt, const char * txtP, const FONT_INFO * font,
		uint8_t fx_type, uint8_t speed, uint8_t delay, uint8_t count );


void fy_init( uint8_t idx, int x, int y, int top_y, int bot_y, char * txt, const char * txtP, const FONT_INFO * font,
		uint8_t fx_type, uint8_t speed, uint8_t delay, uint8_t count );

void GLCD_FX_EVENT( void );


void glcd_fx_counter(
		int x, 												// pozycja x
		int y1, int y2offset,								// y1 - pozycja counter1, y2offset - przesuni�cie w pikselach w stosunku do y1
		int32_t counter1, uint8_t counter2, 				// counter1 - cyfry przed przecinkiem, counter2 - cyfry po przecinku
		uint8_t fx_type,									// fx_type - UWAGA! mo�liwe warto�ci: fx_ld_up albo fx_ld_down
		const char * unit, const char * sep, 				// unit - string jednostek np PSTR("mA"), sep - separator cz�ci ca�kowitej i dziesi�tnej np: PSTR(".") albo zero je�li ma by� niewidoczny
		TCOUNTERPARAMSINIT * cp								// dodatkowe parametry w strukturze
		);



/*
 * 	Tryby wy�wietlania zegarka cyfrowego - clock_mode
 *
 * 	1 - format hh:mm:ss 	- ��cznie 8 znak�w - font dla counter1
 * 	2 - format hh:mm 		- ��cznie 5 znak�w (godziny i minuty) - font dla counter1
 * 	3 - format hh:mm  :ss	- 5 znak�w fontem dla counter1 oraz 3 znaki fontem dla counter2
 * 	4 - format hh:mm   ss	- 5 znak�w fontem dla counter1 oraz 2 znaki fontem dla counter2
 *
 */
void glcd_digital_clock(
		int x, 												// pozycja x
		int y1, int y2offset,								// y1 - pozycja hh:mm:ss, y2offset - przesuni�cie w pikselach w stosunku do y1 dla ss
		uint8_t clock_mode,									// tryb wy�wietlania: 1,2,3,4
		uint8_t hh, uint8_t mm, uint8_t ss, 				// godziny, minuty, sekundy
		uint8_t fx_type,									// fx_type - UWAGA! mo�liwe warto�ci: fx_ld_up albo fx_ld_down
		TCOUNTERPARAMSINIT * cp								// dodatkowe parametry w strukturze
		);



#endif /* MK_GLCD_TEXT_MK_GLCD_SCROLL_TEXT_H_ */














