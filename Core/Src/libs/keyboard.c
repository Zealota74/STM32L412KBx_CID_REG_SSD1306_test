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


static T_KEYB 	keyboard;
static uint8_t	keyAction 	  = action_idle;
static uint8_t	repeatCounter = 0;
static bool 	keyEvent 	  = false;


static INLINE void machine_state_reset(void) {
	keyboard.shortPRESS		= false;
	keyboard.mediumPRESS	= false;
	keyboard.longPRESS		= false;

	keyboard.keyREPEAT		= false;
	keyboard.doublePRESS 	= false;

	keyboard.timeFallFall	= 0;
	keyboard.timeFallRise	= 0;

	keyboard.keysDecoded[0] = 0;
}
static INLINE uint8_t analyze_slope( uint8_t state, uint8_t lastState ) {
//	static bool 	stateDoubleFall	= false;
//	static uint32_t	timeFallFall 	= 0;
	static uint32_t	timeFallRise 	= 0;
//	static uint8_t 	edgeCount		= 0;

	if 		(state == keyPRESSED  && lastState == keyRELEASED) state = stateFALL;
	else if (state == keyRELEASED && lastState == keyPRESSED)  state = stateRISE;
	else if (state == keyRELEASED && lastState == keyRELEASED) state = stateHIGH;
	else if (state == keyPRESSED  && lastState == keyPRESSED)  state = stateLOW;

	switch ( state ) {
		case stateFALL:
			timeFallRise = millis();			// Check time between Fall and Rise
		break;

		case stateRISE:
			timeFallRise 			= millis() - timeFallRise;
			keyboard.timeFallRise	= timeFallRise;
		break;
		default: break;
	}

	return state;
}

static bool keyPressed( void ) {
	keyEvent = false;
	static uint8_t speedPressCounter = 0;

	volatile static uint8_t lastState = keyRELEASED;
	uint8_t pressKeyCheck = sw_get_single_key() ? keyPRESSED : keyRELEASED;
	uint8_t slope 		  = analyze_slope( pressKeyCheck, lastState );

	if ( slope == stateFALL ) {							// Fall edge detected
		if (keyAction == action_idle) {
			keyAction 	  = action_debounce;
			debounceTimer = 10;							// 20 ms na debouncing
			gpio_pin_IMPULSES(DEBUG_PORT0, DEBUG_PIN0, 1);
		}
	} else if ( slope == stateLOW ) {					// State on LOW detected
		switch ( keyAction ) {
			case action_debounce:
				if ( debounceTimer == 0 ) {
					keyAction 	  = action_check;
					debounceTimer = 400;				// 400 ms na sprawdzenie czy to krótkie
					gpio_pin_IMPULSES(DEBUG_PORT0, DEBUG_PIN0, 2);
				} break;
			case action_check:
				if ( debounceTimer == 0 ) {
					keyAction 	  = action_repeat;		// (start funkcji REPEAT)
					debounceTimer = 800;				// 500 ms na sprawdzenie czy klawisz nadal wciśnięty
					repeatCounter = 0;
					gpio_pin_IMPULSES(DEBUG_PORT0, DEBUG_PIN0, 3);
				} break;
			case action_repeat:
				if ( debounceTimer == 0 ) {
					debounceTimer		= 100;			// 100 ms - czas powtarzania funkcji REPEAT
					keyboard.keyREPEAT	= true;			// (start funkcji REPEAT)
					keyEvent 			= true;
					repeatCounter++;
					gpio_pin_IMPULSES(DEBUG_PORT0, DEBUG_PIN0, 4);
				} break;
			default:
			break;
		}
	} else if ( slope == stateRISE ) {										// slope == stateRISE
		if ( keyAction == action_check || keyAction == action_repeat ) {
			debounceTimer = 20;												// 10 ms for RISE debounce
			gpio_pin_IMPULSES(DEBUG_PORT0, DEBUG_PIN0, 5);
		}
	} else if ( slope == stateHIGH ) {										// slope == stateHIGH
		switch ( keyAction ) {

			case action_check:
				if ( debounceTimer == 0 ) {
					if (keyboard.timeFallRise > 120 ) {
						keyboard.shortPRESS = true;		// Zwracamy SHORT, gdyż zwolniliśmy klawisz
						keyEvent 			= true;
						speedPressCounter = 0;
					} else {
						speedPressCounter++;
					}
					keyAction = action_idle;
					gpio_pin_IMPULSES(DEBUG_PORT0, DEBUG_PIN0, 6);
				}										// zanim upłynął czas 500 ms (LONG)
			break;
			case action_repeat:
				if ( repeatCounter == 0 && debounceTimer == 0) {
					keyboard.mediumPRESS = true;
					keyEvent 			 = true;
					keyAction			 = action_idle;
					gpio_pin_IMPULSES(DEBUG_PORT0, DEBUG_PIN0, 7);
					keyAction = action_idle;
				}
			break;
			default:
//				if ( keyAction = action_idle; )
				gpio_pin_IMPULSES(DEBUG_PORT0, DEBUG_PIN0, 8);
				keyAction = action_idle;
			break;
		}

	}
	lastState = pressKeyCheck;
	return keyEvent;
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
	if ( keyPressed() == true) {

		if( keyboard_callback ) {
			keyboard_callback();
			machine_state_reset();
		}
	}
}








/************************** Another ****************************
volatile uint8_t StateKey = 0;

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
***************************************************************/

