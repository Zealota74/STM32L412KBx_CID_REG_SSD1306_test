/*
 * menu.c
 *
 *  Created on: 10.10.2017
 *      Author: simon
 */
#include <stddef.h>						// Potrzebne do typu wchar_t

#include "sw_mcu_conf.h"
#include "../SW_BOARD/sw_gpio.h"
#include "../SW_BOARD/sw_led_blink_debug.h"
#include "../SW_TIMERS/sw_soft_timers.h"

#include "../SW_INPUT/keyboard_v4.h"
#include "../SW_INPUT/encoder_SK.h"

#include "SW_SSD1306/framebuffer.h"
#include "displays.h"
#include "text.h"

#include "sw_spectrum_lib.h"
#include "../SW_RTC/sw_rtc_manage.h"
#include "../SW_IR/sw_irmanage.h"

#include "SW_SSD1306/sw_ssd1306_v1.h"

#include "menu.h"


static T_STRING TextNapisy, TextModes;

static bool	showHideMenuFlag;
static bool	setTimeFlag = SHOW_TIME;
static bool	setTimeType = _setHour;

static int32_t	tmp;
static int8_t	settingModesFlag = _gainSet;


// Ustawiamy parametry tekstu

/**************** Struktura w pamięci RAM przechowująca ustawienia ******************/
static T_SETTINGS settingsSTR;
static const uint8_t RGBeffect[] = {
	12, 0, 60, 0, 10, 4, 0, 0, 30, 30, 30
};
/************************************************************************************/

/******************** Definiujemy napisy z menu w pamięci flash *********************/
const wchar_t Bright[]		= L"Brightness";
const wchar_t Fall[]		= L"Fall time";
const wchar_t Rise[]		= L"Rise time";
const wchar_t Gain[]		= L"Gain";
const wchar_t Peak[]		= L"Peak time";
const wchar_t PeakStop[]	= L"Peak stop";
const wchar_t Effects[]		= L"Effects:";
const wchar_t Colors[]		= L"Colors";

const wchar_t * const Napisy[] = {
	Fall, Rise, Peak, PeakStop, Bright, Gain, Effects, Colors
};

const wchar_t _32bars[]		= L"23 bars";
const wchar_t _16bars[]		= L"16 bars";
const wchar_t _8bars[]		= L"8 bars";
const wchar_t _DotsOR[]		= L"63 bars";
const wchar_t _Eq_avg[]		= L"EQAvg";
const wchar_t _Eq_flat[]	= L"EQFlat";
const wchar_t _2_16kHz[]	= L"2+16kHz";

const wchar_t * const Modes[] = {
	_32bars, _16bars, _8bars, _DotsOR, _Eq_avg, _Eq_flat, _2_16kHz
};
/************************************************************************************/


/************************************************************************************/
void ir_decode_keys( void ) {
	uint16_t tmpKey = sw_ir_return_code();
	if ( tmpKey == IR_KEY_DOWN ||
		 tmpKey == IR_KEY_UP )
		switch_setting_modes();
	else
	if ( tmpKey == IR_KEY_RIGHT ||
		 tmpKey == IR_KEY_LEFT )
		plus_minus();
	else
	if (tmpKey == IR_KEY_OK) {
		if (clock_change_position( 0 )==true)
			sw_update_screen();
	}
	else
	if (tmpKey == IR_KEY_RED)
		rtc_redraw_time( red );
	else
	if (tmpKey == IR_KEY_GREEN)
		rtc_redraw_time( green );
	else
	if (tmpKey == IR_KEY_YELLOW)
		rtc_redraw_time( yellow );
	else
	if (tmpKey == IR_KEY_BLUE)
		rtc_redraw_time( brown );
}
/************************************************************************************/


T_SETTINGS * struct_pointer_ret( void ) { return &settingsSTR; }
/************* Ustawia aktualny zestaw parametrów wyświetlania spektrum *************/
static void set_current_effect( T_SETTINGS *setsRAM ) {
	setsRAM->fallTimeSet = RGBeffect[0];//setsFLASH->fallTimeSet;
	setsRAM->riseTimeSet = RGBeffect[1];//setsFLASH->fallTimeSet;
	setsRAM->peakTimeSet = RGBeffect[2];//setsFLASH->peakTimeSet;
	setsRAM->peakStop	 = RGBeffect[3];//10*setsFLASH->peakStop;

	setsRAM->brightSet	 = RGBeffect[4];//setsFLASH->brightSet;
	setsRAM->gainSet	 = RGBeffect[5];//setsFLASH->gainSet;
	setsRAM->sets		 = RGBeffect[6];//setsFLASH->sets;
	setsRAM->colors		 = RGBeffect[7];//setsFLASH->colors;

	setsRAM->fallAvgTimeSet = RGBeffect[8];
	setsRAM->riseAvgTimeSet = RGBeffect[9];
	setsRAM->peakAvgTimeSet = RGBeffect[10];

	set_get_bright( setsRAM->brightSet );	// Default bright level
	// Default encoder value
	tmp = (int32_t) setsRAM->fallTimeSet;
	sk_set_encoder( (int32_t)tmp );
}
/************************************************************************************/

/*********************** Zarządza zakresami liczb ***********************************/
static int32_t ranges( int32_t min, int32_t max, int32_t value ) {
	if ( value >= max )
		value = max;
	else if ( value <= min )
		value = min;
	return value;
}
static int32_t ranges_ring( int32_t min, int32_t max, int32_t value ) {
	if ( value > max )
		value = min;
	else if ( value < min )
		value = max;
	return value;
}
/************************************************************************************/

/**** Funkcja zwiększa lub zmniejsza zależną wartość po przekroczeniu odp. progów ****************/
static int32_t update_dependant( int32_t min, int32_t max, int32_t value, int32_t *dependant ) {
	static uint8_t tmp;

	if ( (tmp == max)&&(value == min) ) {
		*dependant = *dependant + 1;		// Inkrementujemy
	} else
	if ( (tmp == min)&&(value == max) ) {
		*dependant = *dependant - 1;		// Dekrementujemy
	}
	return tmp = value;
}
/*************************************************************************************************/

/************************* Reaguje na pokręcanie enkoderem **************************/
void plus_minus(void) {
	tmp = sk_get_encoder();
	if ( sw_ir_return_code() == IR_KEY_RIGHT)
		tmp++;
	else
	if( sw_ir_return_code() == IR_KEY_LEFT )
		tmp--;

	if ( setTimeFlag == SHOW_TIME ) {	// Zegarek w trybie wyświetlania
		switch ( settingModesFlag ) {
			case _fallTimeSet:
				tmp = ranges( 0, 100, (int32_t)tmp );
				settingsSTR.fallTimeSet = (int32_t)tmp;
			break;
			case _riseTimeSet:
				tmp = ranges( 0, 100, (int32_t)tmp );
				settingsSTR.riseTimeSet = (int32_t)tmp;
			break;
			case _peakSet:
				tmp = ranges( 0, 254, (int32_t)tmp );
				settingsSTR.peakTimeSet = (int32_t)tmp;
			break;
			case _brightSet:
				tmp = ranges( 0, MAX_BRIGHT, (int32_t)tmp );
				settingsSTR.brightSet = (int32_t)tmp;
				set_get_bright( (int32_t)tmp );
			break;
			case _gainSet:
				tmp = ranges(0, 10, (int32_t)tmp );
				settingsSTR.gainSet = (int32_t)tmp;
			break;
			case _peakStop:
				tmp = ranges( 0, 255, (int32_t)tmp );
				settingsSTR.peakStop = 10*(int32_t)tmp;
			break;
			case _sets:
				tmp = ranges_ring( 0, 5, (int32_t)tmp );
				settingsSTR.sets = (int32_t)tmp;
			break;
			case _themes:
				tmp = ranges_ring( 0, 6, (int32_t)tmp );
				settingsSTR.colors = (int32_t)tmp;
			break;
			default:
			break;
		}
		showMenuTimer = 2000;
		showHideMenuFlag = 0;

//		setHeightBar(21);

		// Przy pokręceniu "TextNapisy" pozostaje bez zmian
		if ( settingModesFlag == _sets ) {
			TextModes.str = (wchar_t *) Modes[ settingsSTR.sets ];
		}
		TextModes.onChange = 1;
	}
	else if ( setTimeFlag == SET_DATE_TIME ) { // Zegarek w trybie ustawiania czasu
		switch( setTimeType ) {
			case _setHour:
				tmp = ranges_ring( 0, 23, (int32_t)tmp );
				update_dependant( 0, 23, (int32_t)tmp, (int32_t *)&SetDateTime.hh );
			break;
			default:	// SETTING_SEC or SETTING_MIN
				tmp = ranges_ring( 0, 59, (int32_t)tmp );
				if ( setTimeType == _setSec )
					update_dependant( 0, 59, (int32_t)tmp, (int32_t *)&SetDateTime.ss);
				else
					update_dependant( 0, 59, (int32_t)tmp, (int32_t *)&SetDateTime.mm);
			break;
		}
	}
	sk_set_encoder(tmp);
}
/****************************************************************************************/

/****************** Przełącza między trybami ustawień, za pomocą klawisza ***************/
void switch_setting_modes( void ) {
	if ( return_keyboard()->switchPressType == SHORT_KEY_PRESS || sw_ir_return_code() == IR_KEY_UP ) {

		sw_led_blink();
		if ( setTimeFlag == SHOW_TIME ) {	// Zegarek w trybie wyświetlania
			if ( showMenuTimer ) {			// Jeśli showTextTimer = 0 to nie zmieniamy
											// ustawień, tylko wyświetlamy
				settingModesFlag = ranges_ring( 0, 7, ++settingModesFlag );
				showHideMenuFlag = 0;
			}
			switch ( settingModesFlag ) {
				case _fallTimeSet:
					tmp = settingsSTR.fallTimeSet;
				break;
				case _riseTimeSet:
					tmp = settingsSTR.riseTimeSet;
				break;
				case _peakSet:
					tmp = settingsSTR.peakTimeSet;
				break;
				case _brightSet:
					tmp = *set_get_bright( 0 );
				break;
				case _gainSet:
					tmp = settingsSTR.gainSet;
				break;
				case _peakStop:
					tmp = settingsSTR.peakStop/10;
				break;
				case _sets:
					tmp = settingsSTR.sets;
				break;
				case _themes:
					tmp = settingsSTR.colors;
				break;
				default:
				break;
			}
			sk_set_encoder( tmp );
		} else if ( setTimeFlag == SET_DATE_TIME ) { // Zegarek w trybie ustawiania czasu
			setTimeType++;
			if (setTimeType > _setSec ) {
				setTimeType = _setHour;
			}
			switch ( setTimeType ) {
				case _setHour:
					tmp = SetDateTime.hh;
				break;
				case _setMin:
					tmp = SetDateTime.mm;
				break;
				case _setSec:
					tmp = SetDateTime.ss;
				break;
				default:
				break;
			}
			sk_set_encoder( tmp );
			return;
		}
	} else if ( return_keyboard()->switchPressType == MEDIUM_KEY_PRESS || sw_ir_return_code()  == IR_KEY_DOWN ) {
		if ( setTimeFlag == SET_DATE_TIME ) return;

		settingModesFlag = ranges_ring( 0, 7, --settingModesFlag );
		showHideMenuFlag = 0;

		switch ( settingModesFlag ) {
			case _fallTimeSet:
				tmp = settingsSTR.fallTimeSet;
			break;
			case _riseTimeSet:
				tmp = settingsSTR.riseTimeSet;
			break;
			case _peakSet:
				tmp = settingsSTR.peakTimeSet;
			break;
			case _brightSet:
				tmp = *set_get_bright( 0 );
			break;
			case _gainSet:
				tmp = settingsSTR.gainSet;
			break;
			case _peakStop:
				tmp = settingsSTR.peakStop/10;
			break;
			case _sets:
				tmp = settingsSTR.sets;
			break;
			case _themes:
				tmp = settingsSTR.colors;
			break;
			default:
			break;
		}
		sk_set_encoder( tmp );

	} else if ( return_keyboard()->switchPressType == LONG_KEY_PRESS ) {
		calculate	^= (1<<0);
		setTimeFlag	^= (1<<0);

		if ( setTimeFlag == SHOW_TIME  ) {
			copy_structures( &SetDateTime, &ShowDateTime );

		} else if ( setTimeFlag == SET_DATE_TIME  ) {
			setTimeType = _setHour;
			copy_structures( &ShowDateTime, &SetDateTime );
			sk_set_encoder( SetDateTime.hh );
		}
		return;
	}
	// Odmierzamy 2s - wyświetlanie menu
	showMenuTimer = 2000;

	// Ładujemy napis do wyświetlania
	TextNapisy.str = (wchar_t *) Napisy[ settingModesFlag ];
	TextNapisy.onChange = 1;

	graphic_clear_last_text( SpectrumBuffer, &TextModes, BlackColor, &GammaRGB);

	if ( settingModesFlag == _sets ) {
		graphic_set_string( &TextModes, L"",      34, 0, &Arial6ptFontInfo_var );
		TextModes.str = (wchar_t *) Modes[ settingsSTR.sets ];
	} else {
		graphic_set_string( &TextModes, L"",      45, 0, &Arial6ptFontInfo_var );
		graphic_puts_int_RAM( SpectrumBuffer, &TextModes, (int32_t)tmp, FONTx1, YellowColor, BlackColor, &GammaRGB );
	}
	TextModes.onChange = 1;

	setHeightBar(31);	// Ustawiamy wysokość spektrum na 16 pikseli
}
/****************************************************************************************/

/********************************* Ustawia parametry wyświetlanych tekstów *************************************/
void set_menu_texts (void ) {
	set_current_effect( &settingsSTR );

	sk_set_encoder( (int32_t)tmp );
	tmp 		 	 = settingsSTR.fallTimeSet;
	settingModesFlag = _fallTimeSet;

	graphic_set_string( &TextNapisy, (wchar_t *) Napisy[ settingModesFlag ], 0, 0, &Arial6ptFontInfo_var );
	graphic_set_string( &TextModes, L"",      45, 0, &Arial6ptFontInfo_var );

}
/***************************************************************************************************************/

/************************************* Funkcja do rysowania czasu na panelu  ***********************************/
void sw_show_menu ( void ) {
//	calculate = 0;
//	clear_buffer_RAM( SpectrumBuffer );
//	clear_spectrum_RAM();
	setHeightBar(15);

	TextNapisy.onChange = 1;
	graphic_puts_RAM( SpectrumBuffer, &TextNapisy, FONTx1, WhiteColor, BlackColor, &GammaRGB );

	if ( settingModesFlag == _sets ) {
		graphic_puts_RAM( SpectrumBuffer, &TextModes, FONTx1, YellowColor, BlackColor, &GammaRGB );
	} else {
		graphic_puts_int_RAM( SpectrumBuffer, &TextModes, tmp, FONTx1, YellowColor, BlackColor, &GammaRGB );
	}
//	calculate = 1;
}
void sw_hide_menu ( void ) {
	set_spectrum_colors( settingsSTR.colors );
	set_spectrum_param ( settingsSTR.sets );
	setHeightBar(30);

	graphic_clear_last_text( SpectrumBuffer, &TextNapisy, BlackColor, &GammaRGB );
	graphic_clear_last_text( SpectrumBuffer, &TextModes, BlackColor, &GammaRGB );
	clear_spectrum_RAM();

	graphic_draw_rectangle_RAM( SpectrumBuffer, 0, 0, 64, 9, BlackColor, &GammaRGB );
}
/***************************************************************************************************************/

/************************ Funkcje callback,owe i zdarzeniowe *****************************/
static void ( *show_menu_event_callback )( void );
static void ( *hide_menu_event_callback )( void );

void register_show_menu_event_callback( void (*callback)( void ) ) {
	show_menu_event_callback = callback;
}
void register_hide_menu_event_callback( void (*callback)( void ) ) {
	hide_menu_event_callback = callback;
}

/******** Obsługa pojawienia się menu ************/
void SW_SHOW_MENU_EVENT( void ) {
	if ( showMenuTimer > 0 && showHideMenuFlag == 0 ) {
		showHideMenuFlag = 1;

		if( show_menu_event_callback ) {
			show_menu_event_callback();
		}
	}
}
/*************************************************/
void SW_HIDE_MENU_EVENT( void ) {
	if ( showMenuTimer == 0 && showHideMenuFlag == 1 ) {
		showHideMenuFlag = 0;

		if( hide_menu_event_callback ) {
			hide_menu_event_callback();
		}
	}
}
/*************************************************/

void sw_menu_init(void) {
	sw_keyboard_init();
	sk_encoder_init();
	sw_ir_init();				// Timer for IRMP

	set_menu_texts();

	sk_register_enc_event_callback( plus_minus );
	register_irDecoded_event_callback( ir_decode_keys );
	register_keyboard_event_callback( switch_setting_modes );

	register_show_menu_event_callback( sw_show_menu );
	register_hide_menu_event_callback( sw_hide_menu );
}
