/*                  e-gadget.header
 * mk_glcd_scroll_text.c
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
#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#elif defined STM32F3 || defined STM32L4
#include "sw_mcu_conf.h"
#include "../../SW_BOARD/sw_gpio.h"
#include "../../SW_TIMERS/sw_soft_timers.h"
#include "../../sw_ltoa.h"
#define PROGMEM
#define pgm_read_byte	*
#define _delay_ms		delay_ms
#define strlen_P		strlen
#endif

#include <string.h>
#include <stdlib.h>

#include "../mk_glcd_base.h"
#include  "mk_glcd_text.h"

#include "../FONTS/mk_fonts.h"


#include "mk_glcd_scroll_text.h"

/*
 *
 *                    |	      xxxxx                      |         xxxxx
 *                    |	    xx     xx                    |       xx     xx
 *                 	  |	  xx         xx                  |     xx         xx
 *                	  |	 x             x                 |    x             x
 *	x                 |	x               x                |	x                x
 *	 x ----------- x -|----------------- x ----------- x |------------------- x -------------
 * 	  xx         xx   |                   xx         xx  |                     xx         xx
 *   	xx     xx     |                     xx     xx    |                       xx     xx
 *     	  xxxxx       |                       xxxxx      |                         xxxxx
 *                    |                                  |
 *                    |<----------- tsinX[] ------------>|
 *
 */


const uint8_t tsin0[] PROGMEM = {4,3,2,2,1,1,0,0,0,1,1,2,2,3,4,5,6,6,7,7,8,8,8,7,7,6,6,5};
const uint8_t tsin1[] PROGMEM = {1,0,1,2,3,2};
const uint8_t tsin2[] PROGMEM = {2,1,1,0,0,0,1,1,2,3,3,4,4,4,3,3};


TMKFX mkfx[ FX_MAX_IDX ];

static TVIEWPORT vcopy;


void fx_stop_now( uint8_t idx ) {
	mkfx[idx].fx_type = fx_not_active;
}

void fx_change_text( uint8_t idx, char * txt ) {

	FONT_INFO xfcopy;
	memcpy( &xfcopy, &currentFont, sizeof(currentFont) );
	setCurrentFont( mkfx[ idx ].font );

	mkfx[ idx ].txtA 			= txt;
	mkfx[ idx ].txt_len			= text_len( txt );

	if( mkfx[ idx ].fx_type == fx_left_out ) {
		mkfx[ idx ].start_x =  mkfx[ idx ].right_x + 1;
		mkfx[ idx ].end_x = mkfx[ idx ].left_x - mkfx[ idx ].txt_len	;
	}
	if( mkfx[ idx ].fx_type == fx_right_out ) {
		mkfx[ idx ].start_x = mkfx[ idx ].left_x - mkfx[ idx ].txt_len	;
		mkfx[ idx ].end_x = mkfx[ idx ].right_x + 1;
	}

	if( mkfx[ idx ].fx_type == fx_left_sin || mkfx[ idx ].fx_type == fx_right_sin || (mkfx[ idx ].fx_type >= fx_left_sin_letters0 && mkfx[ idx ].fx_type <= fx_left_sin_letters2) ) {
		mkfx[ idx ].start_x = mkfx[ idx ].right_x + 1;
		mkfx[ idx ].end_x = mkfx[ idx ].left_x - mkfx[ idx ].txt_len;

		mkfx[ idx ].font_height = currentFont.heightPixels;

		if( mkfx[ idx ].fx_type == fx_right_sin ) {
			mkfx[ idx ].start_x = mkfx[ idx ].left_x - mkfx[ idx ].txt_len;
			mkfx[ idx ].end_x = mkfx[ idx ].right_x;
		}

	}

	memcpy( &currentFont, &xfcopy, sizeof(currentFont) );

}


void fy_init( uint8_t idx, int x, int y, int top_y, int bot_y, char * txt, const char * txtP, const FONT_INFO * font,
		uint8_t fx_type, uint8_t speed, uint8_t delay, uint8_t count ) {

	FONT_INFO fcopy;
	memcpy( &fcopy, &currentFont, sizeof(currentFont) );
	setCurrentFont( font );

	if( count ) count++;

	int txtlen = text_len( txt );

	if( txt ) txtlen = text_len( txt );
	else txtlen = text_len_P( txtP );

	memset( &mkfx[ idx ], 0, sizeof(TMKFX) );

	mkfx[ idx ].txt 			= txt;
	mkfx[ idx ].txtP 			= txtP;
	if( txt ) mkfx[ idx ].ram = 1;
	else mkfx[ idx ].ram = 0;
	mkfx[ idx ].txt_len 		= txtlen;
	mkfx[ idx ].fx_type 		= fx_type;
	mkfx[ idx ].top_y 			= top_y;
	mkfx[ idx ].bot_y 			= bot_y;
	mkfx[ idx ].count			= count;
	mkfx[ idx ].font			= font;
	mkfx[ idx ].y				= y;
	mkfx[ idx ].xp				= (!speed)?1:speed;
	mkfx[ idx ].delay			= (!delay)?1:delay;
	mkfx[ idx ].alidx			= 0;
	mkfx[ idx ].letter[0]		= txt[0];
	mkfx[ idx ].letter[1]		= 0;

	mkfx[ idx ].font_height = 0;
	mkfx[ idx ].txt_x = 0;

	if( fx_type == fx_letters_T2Bin ) {
		mkfx[ idx ].txt_y = top_y - currentFont.heightPixels;
		mkfx[ idx ].left_x = x;
		mkfx[ idx ].txt_x = x;
		mkfx[ idx ].right_x = x + txtlen;
		if( mkfx[ idx ].right_x > GLCD_WIDTH-1 ) mkfx[ idx ].right_x = GLCD_WIDTH-1;
	}

	if( fx_type == fx_letters_T2Bout ) {
		mkfx[ idx ].txt_y = y;
		mkfx[ idx ].left_x = x;
		mkfx[ idx ].txt_x = x;
		mkfx[ idx ].right_x = x + txtlen;
		if( mkfx[ idx ].right_x > GLCD_WIDTH-1 ) mkfx[ idx ].right_x = GLCD_WIDTH-1;

	}

	if( mkfx[ idx ].fx_type == fx_ld_up || mkfx[ idx ].fx_type == fx_ld_down ) {

		mkfx[ idx ].invert_background = 0;
		if( mkfx[ idx ].txt[ 1 ] == 'i' ) mkfx[ idx ].invert_background = 1;

		uint8_t frame = 0;
		if( mkfx[ idx ].txt[ 1 ] == 'f' ) frame = 1;

		mkfx[ idx ].letter[0] = mkfx[ idx ].txt[ 0 ];
		mkfx[ idx ].letter[1] = mkfx[ idx ].txt[ 2 ];

		txtlen = char_len( '0' );

		mkfx[ idx ].txt_y = y;
		mkfx[ idx ].y = y;
		mkfx[ idx ].top_y = y-1;
		mkfx[ idx ].left_x = x-1;
		mkfx[ idx ].txt_x = x;
		mkfx[ idx ].right_x = x + txtlen - currentFont.interspacePixels+1;
		mkfx[ idx ].znacznik1 = 1;

		mkfx[ idx ].txt_len = txtlen;

		if( frame ) {
			glcd_drawRect( mkfx[idx].left_x-1, mkfx[idx].top_y-1, mkfx[idx].right_x - mkfx[idx].left_x +2, currentFont.heightPixels+4, 1 );
		}

	}

	memcpy( &currentFont, &fcopy, sizeof(currentFont) );
}


void fx_init( uint8_t idx, int y, int left_x, int right_x, char * txt, const char * txtP, const FONT_INFO * font,
		uint8_t fx_type, uint8_t speed, uint8_t delay, uint8_t count ) {

	FONT_INFO fcopy;
	memcpy( &fcopy, &currentFont, sizeof(currentFont) );
	setCurrentFont( font );

	if( count ) count++;

	memset( &mkfx[ idx ], 0, sizeof(TMKFX) );

	int txtlen = text_len( txt );

	if( txt ) txtlen = text_len( txt );
	else txtlen = text_len_P( txtP );

	mkfx[ idx ].txt 			= txt;
	mkfx[ idx ].txtP 			= txtP;
	if( txt ) mkfx[ idx ].ram = 1;
	else mkfx[ idx ].ram = 0;
	mkfx[ idx ].txt_len 		= txtlen;
	mkfx[ idx ].fx_type 		= fx_type;
	mkfx[ idx ].left_x 			= left_x;
	mkfx[ idx ].right_x 		= right_x;
	mkfx[ idx ].count			= count;
	mkfx[ idx ].font			= font;
	mkfx[ idx ].y				= y;
	mkfx[ idx ].xp				= (!speed)?1:speed;
	mkfx[ idx ].delay			= (!delay)?1:delay;
	mkfx[ idx ].alidx			= 0;
	mkfx[ idx ].letter[0]		= txt[0];
	mkfx[ idx ].letter[1]		= 0;

	if( fx_type == fx_left ) {
		mkfx[ idx ].start_x = mkfx[ idx ].txt_x = right_x + 1;
		mkfx[ idx ].end_x = left_x;
	}
	if( fx_type == fx_right ) {
		mkfx[ idx ].start_x = mkfx[ idx ].txt_x = left_x - txtlen;
		mkfx[ idx ].end_x = right_x - txtlen;
	}

	if( fx_type == fx_left_out ) {
		mkfx[ idx ].start_x = mkfx[ idx ].txt_x = right_x + 1;
		mkfx[ idx ].end_x = left_x - txtlen;
	}
	if( fx_type == fx_right_out ) {
		mkfx[ idx ].start_x = mkfx[ idx ].txt_x = left_x - txtlen;
		mkfx[ idx ].end_x = right_x + 1;
	}

	if( fx_type == fx_left_letters ) {
		mkfx[ idx ].start_x = mkfx[ idx ].txt_x = right_x + 1;
		mkfx[ idx ].end_x = left_x;
	}

	if( fx_type == fx_right_letters ) {

		if( mkfx[ idx ].ram ) mkfx[ idx ].alidx = strlen(txt)-1;
		else mkfx[ idx ].alidx = strlen_P(txtP)-1;

		uint8_t za;
		char c;
		if( mkfx[ idx ].ram ) c = mkfx[ idx ].txt[ mkfx[ idx ].alidx ];
		else c = pgm_read_byte( &mkfx[ idx ].txtP[ mkfx[ idx ].alidx ] );

		za = char_len( c );

		mkfx[ idx ].start_x = mkfx[ idx ].txt_x = left_x - za;

		mkfx[ idx ].end_x = right_x - za;
	}

	if( fx_type == fx_left_sin || fx_type == fx_right_sin || (fx_type >= fx_left_sin_letters0 && fx_type <= fx_left_sin_letters2) ) {
		mkfx[ idx ].start_x = mkfx[ idx ].txt_x = right_x + 1;
		mkfx[ idx ].end_x = left_x - txtlen;
		mkfx[ idx ].alidx = 0;
		mkfx[ idx ].font_height = currentFont.heightPixels;

		if( fx_type == fx_right_sin ) {
			mkfx[ idx ].start_x = mkfx[ idx ].txt_x = left_x - txtlen;
			mkfx[ idx ].end_x = right_x;
		}

		mkfx[idx].letter[0] = 0;

		for( uint8_t i=0; i<sizeof(tsin0); i++ ) {
			uint8_t b = pgm_read_byte( &tsin0[i] );
			if( b > mkfx[idx].letter[0] ) mkfx[idx].letter[0] = b;
		}


		if( mkfx[idx].fx_type == fx_left_sin_letters0 ) {
			for( uint8_t i=0; i<sizeof(tsin0); i++ ) {
				uint8_t b = pgm_read_byte( &tsin0[i] );
				if( b > mkfx[idx].letter[0] ) mkfx[idx].letter[0] = b;
			}
		}
		else if( mkfx[idx].fx_type == fx_left_sin_letters1 ) {
			for( uint8_t i=0; i<sizeof(tsin1); i++ ) {
				uint8_t b = pgm_read_byte( &tsin1[i] );
				if( b > mkfx[idx].letter[0] ) mkfx[idx].letter[0] = b;
			}
		} else {
			for( uint8_t i=0; i<sizeof(tsin2); i++ ) {
				uint8_t b = pgm_read_byte( &tsin2[i] );
				if( b > mkfx[idx].letter[0] ) mkfx[idx].letter[0] = b;
			}
		}


	}

	memcpy( &currentFont, &fcopy, sizeof(currentFont) );
}





void GLCD_FX_EVENT( void ) {


	static uint8_t ta;
	static uint16_t cnt;
	cnt++;

	ta = text_align;
	text_align = _left;

	uint8_t vcpy = 0;
	if( viewport.active ) {
		memcpy( &vcopy, &viewport, sizeof(viewport) );
		vcpy = 1;
	}

	FONT_INFO xfcopy;
	memcpy( &xfcopy, &currentFont, sizeof(currentFont) );



	for( uint8_t idx = 0; idx < FX_MAX_IDX; idx++ ) {


		if( fx_not_active == mkfx[ idx ].fx_type || 1 == mkfx[ idx ].count ) continue;

		if( !( cnt % mkfx[ idx ].delay ) ) {

			char * atxt = (char*)&mkfx[idx].txt[0];


			uint8_t y = mkfx[idx].y;
			setCurrentFont( mkfx[idx].font );


			switch( mkfx[ idx ].fx_type ) {


				case fx_ld_up:
				{
					glcd_set_viewport( mkfx[idx].left_x, mkfx[idx].top_y, mkfx[idx].right_x - mkfx[idx].left_x, currentFont.heightPixels+2 );

					if( mkfx[idx].znacznik1 ) {
						mkfx[idx].znacznik1 = 0;
						glcd_clear_viewport( mkfx[idx].invert_background );

						glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y, mkfx[idx].letter[0], 2 );
						break;
					} else {

						if( mkfx[idx].letter[0] == mkfx[idx].letter[1] ) break;

						glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y, mkfx[idx].letter[0], 2 );
						glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y+currentFont.heightPixels+1, mkfx[idx].letter[1], 2 );
					}

					if( mkfx[idx].txt_y > mkfx[idx].top_y-currentFont.heightPixels-1 ) {
						if( !(mkfx[idx].letter[0] == ':' || mkfx[idx].letter[0] == ' ') ) mkfx[idx].txt_y-=mkfx[idx].xp;
					}
					else {
						mkfx[idx].fx_type = fx_not_active;
						glcd_put_char( mkfx[idx].txt_x, mkfx[idx].y, mkfx[idx].letter[1], 2 );
						break;
					}

					glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y, mkfx[idx].letter[0], 2 );
					glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y+currentFont.heightPixels+1, mkfx[idx].letter[1], 2 );

					break;
				}

				case fx_ld_down:
				{

					glcd_set_viewport( mkfx[idx].left_x, mkfx[idx].top_y, mkfx[idx].right_x - mkfx[idx].left_x, currentFont.heightPixels+2 );

					if( mkfx[idx].znacznik1 ) {
						mkfx[idx].znacznik1 = 0;
						glcd_clear_viewport( mkfx[ idx ].invert_background );

						glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y, mkfx[idx].letter[0], 2 );
						break;
					} else {
						glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y, mkfx[idx].letter[0], 2 );
						glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y-currentFont.heightPixels-1, mkfx[idx].letter[1], 2 );
					}

					if( mkfx[idx].txt_y < mkfx[idx].top_y+currentFont.heightPixels+1 ) {
						if( !(mkfx[idx].letter[0] == ':' || mkfx[idx].letter[0] == ' ') )  mkfx[idx].txt_y+=mkfx[idx].xp;
					} else {
						mkfx[idx].fx_type = fx_not_active;
						glcd_put_char( mkfx[idx].txt_x, mkfx[idx].y, mkfx[idx].letter[1], 2 );
						break;
					}

					glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y, mkfx[idx].letter[0], 2 );
					glcd_put_char( mkfx[idx].txt_x, mkfx[idx].txt_y-currentFont.heightPixels-1, mkfx[idx].letter[1], 2 );

					break;
				}


				case fx_left:
				{
					glcd_set_viewport_x( mkfx[idx].left_x, mkfx[idx].right_x );
					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y, mkfx[idx].txtP, 2 );
					if( mkfx[idx].txt_x > mkfx[idx].end_x+mkfx[idx].xp ) {
						mkfx[idx].txt_x -= mkfx[idx].xp;
					} else {
						mkfx[idx].txt_x = mkfx[idx].left_x + 1;
						mkfx[idx].fx_type = fx_not_active;
					}
					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y, mkfx[idx].txtP, 2 );
					break;
				}

				case fx_right:
				{
					glcd_set_viewport_x( mkfx[idx].left_x, mkfx[idx].right_x );
					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y, mkfx[idx].txtP, 2 );
					if( mkfx[idx].txt_x < mkfx[idx].end_x-mkfx[idx].xp ) {
						mkfx[idx].txt_x += mkfx[idx].xp;
					} else {
						mkfx[idx].txt_x = mkfx[idx].right_x - mkfx[ idx ].txt_len + mkfx[idx].xp - 1;
						mkfx[idx].fx_type = fx_not_active;
					}
					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y, mkfx[idx].txtP, 2 );
					break;
				}



				case fx_left_out:
				{
					glcd_set_viewport_x( mkfx[idx].left_x, mkfx[idx].right_x );

					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y, mkfx[idx].txtP, 2 );

					if( mkfx[idx].txtA ) {
						mkfx[idx].txt = mkfx[idx].txtA;
						atxt = (char*)&mkfx[idx].txt[0];
						mkfx[idx].txtA = 0;
						glcd_fillRect( mkfx[idx].left_x, mkfx[idx].y, 1+ mkfx[idx].left_x+(mkfx[idx].right_x-mkfx[idx].left_x), currentFont.heightPixels ,0 );
					}

					if( mkfx[idx].txt_x > mkfx[idx].end_x ) {
						mkfx[idx].txt_x -= mkfx[idx].xp;
					} else {
						if( mkfx[idx].count > 1 ) {
							mkfx[idx].count--;
							mkfx[idx].txt_x = mkfx[idx].start_x;
						} else mkfx[idx].txt_x = mkfx[idx].start_x;
					}

					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y, mkfx[idx].txtP, 2 );

					break;
				}

				case fx_right_out:
				{
					glcd_set_viewport_x( mkfx[idx].left_x, mkfx[idx].right_x );

					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y, mkfx[idx].txtP, 2 );

					if( mkfx[idx].txtA ) {
						mkfx[idx].txt = mkfx[idx].txtA;
						atxt = (char*)&mkfx[idx].txt[0];
						mkfx[idx].txtA = 0;
						glcd_fillRect( mkfx[idx].left_x, mkfx[idx].y, 1+ mkfx[idx].left_x+(mkfx[idx].right_x-mkfx[idx].left_x), currentFont.heightPixels ,0 );
					}

					if( mkfx[idx].txt_x < mkfx[idx].end_x ) {
						mkfx[idx].txt_x += mkfx[idx].xp;
					} else {
						if( mkfx[idx].count > 1 ) {
							mkfx[idx].count--;
							mkfx[idx].txt_x = mkfx[idx].start_x;
						} else mkfx[idx].txt_x = mkfx[idx].start_x;
					}

					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y, mkfx[idx].txtP, 2 );

					break;
				}


				case fx_left_letters:
				{
					glcd_set_viewport_x( mkfx[idx].left_x, mkfx[idx].right_x );

					if( mkfx[ idx ].ram ) mkfx[idx].letter[0] = mkfx[idx].txt[ mkfx[idx].alidx ];
					else mkfx[idx].letter[0] = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

					atxt = &mkfx[idx].letter[0];

					if( mkfx[idx].txt_x > mkfx[idx].end_x+mkfx[idx].xp ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );

					if( mkfx[idx].txt_x > mkfx[idx].end_x+mkfx[idx].xp ) {
						mkfx[idx].txt_x -= mkfx[idx].xp;
					} else {
						glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
						mkfx[idx].txt_x = mkfx[idx].end_x+1;
						glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );

						mkfx[idx].txt_x = mkfx[idx].right_x + 1;

						mkfx[idx].end_x += text_len( mkfx[idx].letter );
						mkfx[idx].alidx++;

						char literka;
						uint8_t w=0;
						while(1) {

							if( mkfx[ idx ].ram ) literka = mkfx[idx].txt[ mkfx[idx].alidx ];
							else literka = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

							if( literka == ' ' ) {
								w = char_len( ' ' );
								mkfx[idx].end_x += w;
							} else break;
							mkfx[idx].alidx++;
						}

						char znak;
						if( mkfx[ idx ].ram ) znak = mkfx[idx].txt[ mkfx[idx].alidx ];
						else znak = pgm_read_byte( &mkfx[idx].txt[ mkfx[idx].alidx ] );

						if( mkfx[idx].end_x > mkfx[idx].right_x || !znak ) mkfx[idx].fx_type = 0;

						if( mkfx[ idx ].ram ) mkfx[idx].letter[0] = mkfx[idx].txt[ mkfx[idx].alidx ];
						else mkfx[idx].letter[0] = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

					}
					glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					break;
				}



				case fx_right_letters:
				{
					glcd_set_viewport_x( mkfx[idx].left_x, mkfx[idx].right_x );

					if( mkfx[ idx ].ram ) mkfx[idx].letter[0] = mkfx[idx].txt[ mkfx[idx].alidx ];
					else mkfx[idx].letter[0] = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

					atxt = &mkfx[idx].letter[0];

					if( mkfx[idx].txt_x < mkfx[idx].end_x-mkfx[idx].xp ) glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );

					if( mkfx[idx].txt_x < mkfx[idx].end_x-mkfx[idx].xp ) {
						mkfx[idx].txt_x += mkfx[idx].xp;
					} else {
						glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
						mkfx[idx].txt_x = mkfx[idx].end_x+1;
						glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );

						mkfx[idx].alidx--;

						uint8_t za;
						char c;
						if( mkfx[ idx ].ram ) c = mkfx[ idx ].txt[ mkfx[ idx ].alidx ];
						else c = pgm_read_byte( &mkfx[ idx ].txtP[ mkfx[ idx ].alidx ] );

						za = char_len( c );

						mkfx[idx].end_x -= za;
						mkfx[idx].txt_x = mkfx[idx].left_x - za;

						char literka;
						while(1) {

							if( mkfx[ idx ].ram ) literka = mkfx[idx].txt[ mkfx[idx].alidx ];
							else literka = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

							if( literka == ' ' ) {

								if( mkfx[ idx ].ram ) c = mkfx[ idx ].txt[ mkfx[ idx ].alidx-1 ];
								else c = pgm_read_byte( &mkfx[ idx ].txtP[ mkfx[ idx ].alidx-1 ] );

								mkfx[idx].end_x -= char_len( c );
							} else break;
							mkfx[idx].alidx--;
						}

						char znak;
						if( mkfx[ idx ].ram ) znak = mkfx[idx].txt[ mkfx[idx].alidx ];
						else znak = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

						if( mkfx[idx].end_x < mkfx[idx].left_x || !znak ) mkfx[idx].fx_type = 0;

						if( mkfx[ idx ].ram ) mkfx[idx].letter[0] = mkfx[idx].txt[ mkfx[idx].alidx ];
						else mkfx[idx].letter[0] = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

					}
					glcd_puts( mkfx[idx].txt_x, y, atxt, 2 );
					break;
				}



				case fx_left_sin:
				{
					glcd_set_viewport_x( mkfx[idx].left_x, mkfx[idx].right_x );
					uint8_t * didx = (uint8_t*)&mkfx[idx].letter[1];

					uint8_t ay = pgm_read_byte( &tsin0[ *didx ] );

					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y+ay, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y+ay, mkfx[idx].txtP, 2 );

					if( mkfx[idx].txtA ) {
						mkfx[idx].txt = mkfx[idx].txtA;
						atxt = (char*)&mkfx[idx].txt[0];
						mkfx[idx].txtA = 0;
						glcd_fillRect( mkfx[idx].left_x, mkfx[idx].y, 1+ mkfx[idx].left_x+(mkfx[idx].right_x-mkfx[idx].left_x), currentFont.heightPixels+mkfx[idx].letter[0] ,0 );
					}

					if( mkfx[idx].txt_x > mkfx[idx].end_x ) {
						mkfx[idx].txt_x -= mkfx[idx].xp;
					} else {
						if( mkfx[idx].count > 1 ) {
							mkfx[idx].count--;
							mkfx[idx].txt_x = mkfx[idx].start_x;
						} else mkfx[idx].txt_x = mkfx[idx].start_x;
					}
					ay = pgm_read_byte( &tsin0[ mkfx[idx].alidx ] );

					*didx = mkfx[idx].alidx;

					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y+ay, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y+ay, mkfx[idx].txtP, 2 );

					if( ++mkfx[idx].alidx > sizeof(tsin0)-1 ) mkfx[idx].alidx = 0;
					break;
				}


				case fx_right_sin:
				{
					glcd_set_viewport_x( mkfx[idx].left_x, mkfx[idx].right_x );
					uint8_t * didx = (uint8_t*)&mkfx[idx].letter[1];

					uint8_t ay = pgm_read_byte( &tsin0[ *didx ] );

					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y+ay, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y+ay, mkfx[idx].txtP, 2 );

					if( mkfx[idx].txtA ) {
						mkfx[idx].txt = mkfx[idx].txtA;
						atxt = (char*)&mkfx[idx].txt[0];
						mkfx[idx].txtA = 0;

						glcd_fillRect( mkfx[idx].left_x, mkfx[idx].y, 1+ mkfx[idx].left_x+(mkfx[idx].right_x-mkfx[idx].left_x), currentFont.heightPixels+mkfx[idx].letter[0] ,0 );
					}

					if( mkfx[idx].txt_x < mkfx[idx].end_x ) {
						mkfx[idx].txt_x += mkfx[idx].xp;
					} else {
						if( mkfx[idx].count > 1 ) {
							mkfx[idx].count--;
							mkfx[idx].txt_x = mkfx[idx].start_x;
						} else mkfx[idx].txt_x = mkfx[idx].start_x;
					}
					ay = pgm_read_byte( &tsin0[ mkfx[idx].alidx ] );

					*didx = mkfx[idx].alidx;

					if( mkfx[ idx ].ram ) glcd_puts( mkfx[idx].txt_x, y+ay, atxt, 2 );
					else glcd_puts_P( mkfx[idx].txt_x, y+ay, mkfx[idx].txtP, 2 );

					if( ++mkfx[idx].alidx > sizeof(tsin0)-1 ) mkfx[idx].alidx = 0;
					break;
				}


				case fx_left_sin_letters0 ... fx_left_sin_letters2:
				{
					glcd_set_viewport_x( mkfx[idx].left_x, mkfx[idx].right_x );

					const uint8_t * stab = tsin2;
					uint8_t so = sizeof( tsin2 )-1;
					if( mkfx[idx].fx_type == fx_left_sin_letters0 ) {
						stab = tsin0;
						so = sizeof( tsin0 )-1;
					}
					else if( mkfx[idx].fx_type == fx_left_sin_letters1 ) {
						stab = tsin1;
						so = sizeof( tsin1 )-1;
					}

					int ay;
					uint8_t * didx = (uint8_t*)&mkfx[idx].letter[1];

					cur_x = mkfx[idx].txt_x;

					uint8_t za;
					if( mkfx[ idx ].ram ) za = strlen(mkfx[idx].txt);
					else za = strlen_P(mkfx[idx].txtP);

					char c;

					for( int k=0; k<za; k++ ) {
						ay = mkfx[idx].y + pgm_read_byte( &stab[ *didx ] );

						if( mkfx[ idx ].ram ) c = mkfx[idx].txt[ k ];
						else c = pgm_read_byte( &mkfx[idx].txtP[ k ] );

						glcd_put_char( cur_x, ay, c, 2 );

						if( ++(*didx) > so ) *didx = 0;
					}

					if( mkfx[idx].txtA ) {
						mkfx[idx].txt = mkfx[idx].txtA;
						mkfx[idx].txtA = 0;
						glcd_fillRect( mkfx[idx].left_x, mkfx[idx].y, 1+ mkfx[idx].left_x+(mkfx[idx].right_x-mkfx[idx].left_x), currentFont.heightPixels+mkfx[idx].letter[0] ,0 );
					}

					if( mkfx[idx].txt_x > mkfx[idx].end_x ) {
						mkfx[idx].txt_x -= mkfx[idx].xp;
					} else {
						if( mkfx[idx].count > 1 ) {
							mkfx[idx].count--;
							mkfx[idx].txt_x = mkfx[idx].start_x;
						} else mkfx[idx].txt_x = mkfx[idx].start_x;
					}

					int cidx;
					cur_x = mkfx[idx].txt_x;

					cidx = mkfx[idx].alidx;
					*didx = mkfx[idx].alidx;

					if( mkfx[ idx ].ram ) za = strlen(mkfx[idx].txt);
					else za = strlen_P(mkfx[idx].txtP);

					for( int k=0; k<za; k++ ) {
						ay = mkfx[idx].y + pgm_read_byte( &stab[ cidx ] );

						if( mkfx[ idx ].ram ) c = mkfx[idx].txt[ k ];
						else c = pgm_read_byte( &mkfx[idx].txtP[ k ] );

						glcd_put_char( cur_x, ay, c, 2 );

						if( ++cidx > so ) cidx = 0;
					}

					mkfx[idx].alidx++;
					if( mkfx[idx].alidx > so ) mkfx[idx].alidx = 0;

					break;
				}


				case fx_letters_T2Bin:
				{
					glcd_set_viewport( mkfx[idx].left_x, mkfx[idx].top_y, mkfx[idx].right_x - mkfx[idx].left_x, mkfx[idx].bot_y - mkfx[idx].top_y );

					if( mkfx[ idx ].ram ) mkfx[idx].letter[0] = mkfx[idx].txt[ mkfx[idx].alidx ];
					else mkfx[idx].letter[0] = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

					atxt = &mkfx[idx].letter[0];

					if( mkfx[idx].txt_y < mkfx[idx].y-mkfx[idx].xp ) glcd_puts( mkfx[idx].txt_x, mkfx[idx].txt_y, atxt, 2 );

					if( mkfx[idx].txt_y < mkfx[idx].y-mkfx[idx].xp ) {
						mkfx[idx].txt_y += mkfx[idx].xp;
					} else {
						glcd_puts( mkfx[idx].txt_x, mkfx[idx].txt_y, atxt, 2 );
						mkfx[idx].txt_y = mkfx[idx].y;
						glcd_puts( mkfx[idx].txt_x, mkfx[idx].txt_y, atxt, 2 );

						mkfx[idx].txt_x += text_len(atxt);

						mkfx[idx].txt_y = mkfx[idx].top_y - currentFont.heightPixels;
						mkfx[idx].alidx++;

						char literka;
						while(1) {

							if( mkfx[ idx ].ram ) literka = mkfx[idx].txt[ mkfx[idx].alidx ];
							else literka = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

							if( literka == ' ' ) {
								mkfx[idx].txt_x += char_len(' ');
								mkfx[idx].alidx++;
							} else break;
						}

					}

					glcd_puts( mkfx[idx].txt_x, mkfx[idx].txt_y, atxt, 2 );

					uint8_t za;
					if( mkfx[ idx ].ram ) za = strlen(mkfx[idx].txt)-1;
					else za = strlen_P(mkfx[idx].txtP)-1;

					if( mkfx[idx].alidx > za ) mkfx[idx].fx_type = fx_not_active;

					if( mkfx[idx].txt_x > mkfx[idx].right_x ) mkfx[idx].fx_type = fx_not_active;

					break;
				}



				case fx_letters_T2Bout:
				{

					glcd_set_viewport( mkfx[idx].left_x, mkfx[idx].top_y, mkfx[idx].right_x - mkfx[idx].left_x, mkfx[idx].bot_y - mkfx[idx].top_y );

					if( mkfx[ idx ].ram ) mkfx[idx].letter[0] = mkfx[idx].txt[ mkfx[idx].alidx ];
					else mkfx[idx].letter[0] = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

					atxt = &mkfx[idx].letter[0];

					glcd_puts( mkfx[idx].txt_x, mkfx[idx].txt_y, atxt, 2 );

					if( mkfx[idx].txt_y < mkfx[idx].bot_y ) {
						mkfx[idx].txt_y += mkfx[idx].xp;
					} else {
						mkfx[idx].txt_x += text_len(atxt);
						mkfx[idx].txt_y = mkfx[idx].y;
						mkfx[idx].alidx++;

						if( mkfx[ idx ].ram ) mkfx[idx].letter[0] = mkfx[idx].txt[ mkfx[idx].alidx ];
						else mkfx[idx].letter[0] = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

						char literka;
						while(1) {

							if( mkfx[ idx ].ram ) literka = mkfx[idx].txt[ mkfx[idx].alidx ];
							else literka = pgm_read_byte( &mkfx[idx].txtP[ mkfx[idx].alidx ] );

							if( literka == ' ' ) {
								mkfx[idx].txt_x += char_len(' ');
								mkfx[idx].alidx++;
							} else break;
						}

					}

					if( (mkfx[idx].txt_x+text_len(atxt)) < mkfx[idx].right_x+char_len(mkfx[idx].letter[0]) ) {
						if( mkfx[idx].txt_y > mkfx[idx].y ) glcd_puts( mkfx[idx].txt_x, mkfx[idx].txt_y, atxt, 2 );
					}
					else mkfx[idx].fx_type = fx_not_active;

					uint8_t za;
					if( mkfx[ idx ].ram ) za = strlen(mkfx[idx].txt)-1;
					else za = strlen_P(mkfx[idx].txtP)-1;

					if( mkfx[idx].alidx > za ) mkfx[idx].fx_type = fx_not_active;

					if( mkfx[idx].txt_x > mkfx[idx].right_x ) mkfx[idx].fx_type = fx_not_active;
					break;
				}



			}	// end switch()

		}    // end if delay

	}	// end for(...

	if( vcpy ) memcpy( &viewport, &vcopy, sizeof(viewport) );
	else glcd_reset_viewport();

	memcpy( &currentFont, &xfcopy, sizeof(currentFont) );

	text_align = ta;

}


/*
 * 	Tryby wy�wietlania zegarka cyfrowego - clock_mode
 *
 * 	1 - format hh:mm:ss 	- ��cznie 8 znak�w - font dla counter1
 * 	2 - format hh:mm 		- ��cznie 5 znak�w (godziny i minuty) - font dla counter1
 * 	3 - format hh:mm  :ss	- 5 znak�w fontem dla counter1 oraz 3 znaki fonterm dla counter2
 * 	4 - format hh:mm   ss	- 5 znak�w fontem dla counter1 oraz 2 znaki fonterm dla counter2
 *
 */
void glcd_digital_clock(
		int x, 												// pozycja x
		int y1, int y2offset,								// y1 - pozycja hh:mm:ss, y2offset - przesuni�cie w pikselach w stosunku do y1 dla ss
		uint8_t clock_mode,									// tryb wy�wietlania: 1,2,3,4
		uint8_t hh, uint8_t mm, uint8_t ss, 				// godziny, minuty, sekundy
		uint8_t fx_type,									// fx_type - UWAGA! mo�liwe warto�ci: fx_ld_up albo fx_ld_down
		TCOUNTERPARAMSINIT * cp								// dodatkowe parametry w strukturze
		) {

	if( !clock_mode ) return;

	cp->as_clock = clock_mode;

	if( clock_mode < 3 ) {

		if( 1 == clock_mode ) {
			cp->len1 = 8;
			glcd_fx_counter( x, y1, 0, (hh*10000ul)+(mm*100ul)+ss, 0, fx_type, 0, 0, cp );
		} else {
			cp->len1 = 5;
			cp->len2 = 0;
			glcd_fx_counter( x, y1, 0, (hh*100ul)+mm, 0, fx_type, 0, 0, cp );
		}
	} else {
		cp->len1 = 5;
		if( 4 == clock_mode ) cp->len2 = 2; else cp->len2 = 3;
		glcd_fx_counter( x, y1, y2offset, (hh*100)+mm, ss, fx_type, 0, 0, cp );
	}

}



void glcd_fx_counter(
		int x, 												// pozycja x
		int y1, int y2offset,								// y1 - pozycja counter1, y2offset - przesuni�cie w pikselach w stosunku do y1
		int32_t counter1, uint8_t counter2, 				// counter1 - cyfry przed przecinkiem, counter2 - cyfry po przecinku
		uint8_t fx_type,									// fx_type - UWAGA! mo�liwe warto�ci: fx_ld_up albo fx_ld_down
		const char * unit, const char * sep, 				// unit - string jednostek np PSTR("mA"), sep - separator cz�ci ca�kowitej i dziesi�tnej np: PSTR(".") albo zero je�li ma by� niewidoczny
		TCOUNTERPARAMSINIT * cp
		) {

	static FONT_INFO fcopy;

	static char lbuf1[ FX_COUNTER1_LEN_MAX + 1 ];
	static char lbuf2[ FX_COUNTER2_LEN_MAX + 1 ];

	char tbuf1[ FX_COUNTER1_LEN_MAX + 1 ];
	char tbuf2[ FX_COUNTER2_LEN_MAX + 1 ];

	memset( tbuf1, 0, sizeof(tbuf1) );
	memset( tbuf2, 0, sizeof(tbuf2) );

	if( cp->as_clock ) cp->leading_zero = 1;

	char lz = ' ';
	if( cp->leading_zero ) lz = '0';

	if( 1 == cp->as_clock ) {
		cp->len1 = 8;
	}
	else if( 2 == cp->as_clock ) {
		cp->len1 = 5;
		cp->len2 = 0;
	}
	else if( 3 == cp->as_clock ) {
		cp->len1 = 5;
		cp->len2 = 3;
	}
	else if( 4 == cp->as_clock ) {
		cp->len1 = 5;
		cp->len2 = 2;
	}

	mkitoa( counter1, tbuf1, cp->len1, lz );
	mkitoa( counter2, tbuf2, cp->len2, lz );

	if( 1 == cp->as_clock ) {
		tbuf1[0] = tbuf1[2];
		tbuf1[1] = tbuf1[3];
		tbuf1[2] = cp->clock_separator;
		tbuf1[3] = tbuf1[4];
		tbuf1[4] = tbuf1[5];
		tbuf1[5] = cp->clock_separator;
	}
	else if( 2 == cp->as_clock ) {
		tbuf1[0] = tbuf1[1];
		tbuf1[1] = tbuf1[2];
		tbuf1[2] = cp->clock_separator;
	}
	else if( 3 == cp->as_clock ) {
		tbuf1[0] = tbuf1[1];
		tbuf1[1] = tbuf1[2];
		tbuf1[2] = cp->clock_separator;

		tbuf2[0] = cp->clock_separator;
	}
	else if( 4 == cp->as_clock ) {
		tbuf1[0] = tbuf1[1];
		tbuf1[1] = tbuf1[2];
		tbuf1[2] = cp->clock_separator;
	}

	memcpy( &fcopy, &currentFont, sizeof(currentFont) );

	setCurrentFont( cp->font1 );
	uint8_t f1width = char_len('0') - currentFont.interspacePixels + 1;
	uint8_t f2width = 0;
	if( cp->atr1 == 'f' ) f1width++;

	uint8_t is2 = 0;
	uint8_t unit_width = 0;
	uint8_t sep_width=0;

	if( unit ) unit_width = text_len_P(unit);

	setCurrentFont( cp->font2 );
	if( sep ) sep_width = text_len_P(sep);


	if( cp->font2 ) {

		if( unit ) unit_width = text_len_P(unit);

		f2width = char_len('0') - currentFont.interspacePixels + 1;
		is2 = currentFont.interspacePixels;
		if( cp->atr2 == 'f' ) f2width++;
	}

	if( _center == cp->align ) x = x/2 - ( cp->len1*f1width + cp->len1*cp->interspace_px1 + cp->len2*f2width + cp->len2*cp->interspace_px2 + unit_width + sep_width + is2 )/2 + 1;
	else if( _right == cp->align ) x = x - ( cp->len1*f1width + cp->len1*cp->interspace_px1 + cp->len2*f2width + cp->len2*cp->interspace_px2 + unit_width + sep_width + is2 -1 ) ;

	char bf[3];
	uint8_t idx = FX_MAX_IDX-1;
	for( uint8_t i=0; i<cp->len1; i++ ) {
		bf[0] = lbuf1[i];
		bf[1] = cp->atr1;
		if( lbuf1[i] != tbuf1[i] ) {
			bf[2] = tbuf1[i];
			fy_init( idx, x ,y1, 0, 64, bf,0, cp->font1, fx_type, cp->speed, cp->delay, 0 );
		} else {
			bf[2] = lbuf1[i];
			fy_init( idx, x ,y1, 0, 64, bf,0, cp->font1, fx_ld_up, cp->speed, cp->delay, 0 );
		}
		if( idx ) idx--;
		x += f1width+cp->interspace_px1;
	}

	x++;

	if( cp->font2 ) {
	if( sep ) {
		setCurrentFont( cp->font2 );
		glcd_puts_P(x, y1+y2offset, sep, 1 );
		x += f2width;// - in2;
	}

	for( uint8_t i=0; i<cp->len2; i++ ) {
		bf[0] = lbuf2[i];
		bf[1] = cp->atr2;
		if( lbuf2[i] != tbuf2[i] ) {
			bf[2] = tbuf2[i];
			fy_init( idx, x ,y1+y2offset, 0, 64, bf,0, cp->font2, fx_type, cp->speed, cp->delay, 0 );
		} else {
			bf[2] = lbuf2[i];
			fy_init( idx, x ,y1+y2offset, 0, 64, bf,0, cp->font2, fx_ld_up, cp->speed, cp->delay, 0 );
		}
		if( idx ) idx--;
		x += f2width+cp->interspace_px2;
	}

	x++;
	}

	if( unit ) {
		if( cp->font2 && cp->font1 != cp->font2 ) {
			setCurrentFont( cp->font2 );

			if( cp->atr1 == 'f' ) glcd_puts_P(x, y1 + ( y1 - y1 + currentFont.heightPixels ) - 2, unit, 1 );
			else glcd_puts_P(x, y1+y2offset /*+ (( y1 - y1 + currentFont.heightPixels )/2) - 2*/, unit, 1 );
		} else {
			setCurrentFont( cp->font1 );
			glcd_puts_P(x, y1+y2offset, unit, 1 );
		}

	}

	memset( lbuf1, 0, sizeof(lbuf1) );
	memset( lbuf2, 0, sizeof(lbuf2) );

	mkitoa( counter1, lbuf1, cp->len1, lz );
	mkitoa( counter2, lbuf2, cp->len2, lz );

	if( 1 == cp->as_clock ) {
		lbuf1[0] = lbuf1[2];
		lbuf1[1] = lbuf1[3];
		lbuf1[2] = cp->clock_separator;
		lbuf1[3] = lbuf1[4];
		lbuf1[4] = lbuf1[5];
		lbuf1[5] = cp->clock_separator;
	}
	else if( 2 == cp->as_clock ) {
		lbuf1[0] = lbuf1[1];
		lbuf1[1] = lbuf1[2];
		lbuf1[2] = cp->clock_separator;
	}
	else if( 3 == cp->as_clock ) {
		lbuf1[0] = lbuf1[1];
		lbuf1[1] = lbuf1[2];
		lbuf1[2] = cp->clock_separator;

		lbuf2[0] = cp->clock_separator;
	}
	else if( 4 == cp->as_clock ) {
		lbuf1[0] = lbuf1[1];
		lbuf1[1] = lbuf1[2];
		lbuf1[2] = cp->clock_separator;
	}


	memcpy( &currentFont, &fcopy, sizeof(currentFont) );


}














