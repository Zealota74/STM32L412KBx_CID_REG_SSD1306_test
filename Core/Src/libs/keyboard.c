/*
 * keyboard.c
 *
 *  Created on: 12 sie 2021
 *      Author: simon
 */

#include "sw_mcu_conf.h"

#include "SW_BOARD/sw_gpio.h"
#include "SW_TIMERS/sw_soft_timers.h"


#include "keyboard.h"

/************************** Another ****************************/
volatile uint8_t StateKey = 0;

enum {
	stateFALL = 1, stateRISE = 2
};
enum {
	keyPRESSED = (1 << 0), keyRELEASED = (1 << 1), keyFULL = keyPRESSED | keyRELEASED
};
void key_handler( void ) {
	if (softTimer2 > 0) {
		return;
	} else {
		softTimer2 = 50;
	}

	volatile static uint8_t lastState = keyRELEASED;
	uint8_t state;
	state = sw_get_single_key() ? keyPRESSED : keyRELEASED;

//	switch ( state | lastState ) {
//		case (keyPRESSED ):
//			StateKey = stateFALL;
//			break;
//		case (keyRELEASED):
//			StateKey = stateFALL;
//			break;
//		case (keyFULL):
//			StateKey = stateFALL;
//			break;
//		default:
//			break;
//	}
	if ( state == keyPRESSED  && lastState == keyRELEASED )
		StateKey = stateFALL;
	else
	if ( state == keyRELEASED && lastState == keyPRESSED )
		StateKey = stateRISE;
//	else
//	if ( State == 0 && lastState == 3 )
//		StateKey = 3;

	lastState = state;
}

uint8_t zmienna1, zmienna2, zmienna3;
void key_proc(void) {
	if ( StateKey == 1 ) {
		zmienna1++;
		StateKey = 0;
//		sw_led_blink();
//		TEXT_display_float( 0, 0,  zmienna2, &TextX );
//		sw_ssd1306_display();
	} else
	if ( StateKey == 2 ) {
		zmienna1++;
		StateKey = 0;
//		TEXT_display_float( 0, 16,  zmienna1, &TextY );
//		sw_ssd1306_display();
	} else
	if ( StateKey == 3 ) {
		zmienna2++;
		StateKey = 0;
//		TEXT_display_float( 0, 32,  zmienna2, &TextZ );
//		sw_ssd1306_display();
	}
}
/***************************************************************/



static T_KEYB keyboard;
static uint8_t keyAction = action_idle;

static INLINE void machine_state_reset(void) {
	keyboard.pressType		= IDLE;
	keyboard.keysDecoded[0] = 0;
}

static INLINE uint8_t analyze_slope( uint8_t state, uint8_t lastState ) {
	if ( state == keyPRESSED  && lastState == keyRELEASED )
		return stateFALL;
	else
	if ( state == keyRELEASED && lastState == keyPRESSED )
		return stateRISE;
	return 0;
}

static void keyPressed( void ) {
	volatile static uint8_t lastState = keyRELEASED;

	uint8_t pressKeyCheck = sw_get_single_key() ? keyPRESSED : keyRELEASED;
	uint8_t slope 		  = analyze_slope( pressKeyCheck, lastState );

	if ( pressKeyCheck == keyPRESSED ) {

		switch ( keyAction ) {
			case action_idle:
				keyAction 		= action_debounce;
				debounceTimer 	= 50;					// 50 ms na debouncing
				return;
			case action_debounce:
				if ( debounceTimer == 0 ) {
					keyAction 		= action_check;
					debounceTimer	= 400;				// 1000 ms na sprawdzenie czy to krótkie
				}										// czy długie przyciśnięcie
				return;
			case action_check:
				if ( debounceTimer == 0 ) {
					debounceTimer 		= 1000;				// 300 ms na sprawdzenie czy klawisz nadal wciśnięty
					keyAction 			= action_repeat;	// (start funkcji REPEAT)
					keyboard.pressType	= LONG;
				}
				return;
			case action_repeat:
				if ( debounceTimer == 0 ) {
					debounceTimer		= 100;			// 100 ms - czas powtarzania funkcji REPEAT
					keyboard.pressType	= REPEAT;		// (start funkcji REPEAT)
				}
				return;
			default:
				break;
		}
	} else {
		if ( keyAction == action_check ) {
			if (debounceTimer) {
				keyAction 			= action_idle;
				keyboard.pressType	= SHORT;		// Zwracamy SHORT, gdyż zwolniliśmy klawisz
			}										// zanim upłynął czas 500 ms (LONG)
		} else {
			keyAction = action_idle;
		}
	}
	lastState = pressKeyCheck;
}
/********************************************************************************************/

T_KEYB * keyboard_ptr(void) {
	return &keyboard;
}

static void ( *keyboard_callback )( void );
void register_keyboard_callback( void (*callback)( void) ) {
	keyboard_callback = callback;
}

void SW_KEYBOARD_EVENT( void ) {
	keyPressed();
	if ( keyboard.pressType != 0 ) {
		if( keyboard_callback ) {
			keyboard_callback();
			machine_state_reset();
		}
	}
}

