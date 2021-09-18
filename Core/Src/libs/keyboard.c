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

// ---------------------- Variables and constants -----------------------
static const T_KEYB_OUT KeysTab[ NUM_KEYS ] = {
	{ BTN1_PORT, BTN1_PIN, key2_decoded, {0, 0} },
	{ BTN2_PORT, BTN2_PIN, key1_decoded, {0, 0} }
};

static KEYB_t  		keyboard;
static KEY_ACTION_t keyAction = action_idle;

static uint8_t repeatCounter 	 = 0;
static uint8_t speedPressCounter = 0;

static uint16_t timeFallRise  	 = 0;	// Time: Fall and Rise
static uint16_t timeFall2Rise 	 = 0;	// Time: first fall and last rise
// ----------------------------------------------------------------------


static INLINE void machine_state_reset(void) {
	keyboard.keyEvent = NO_EVENT;
	keyboard.keyCoded = NO_KEY;
}

/***************************** Detect slopes ****************************/
static INLINE uint8_t analyze_slope( uint8_t state, uint8_t lastState ) {
	if 		(state == keyPRESSED  && lastState == keyRELEASED)
		state = stateFALL;
	else if (state == keyPRESSED  && lastState == keyPRESSED)
		state = stateLOW;
	else if (state == keyRELEASED && lastState == keyPRESSED)
		state = stateRISE;
	else if (state == keyRELEASED && lastState == keyRELEASED)
		state = stateHIGH;

	switch ( state ) {
		case stateFALL:
			timeFallRise = millis();
			if (speedPressCounter++ == 0) {
				timeFall2Rise = millis();
			}
			break;
		case stateRISE:
			timeFallRise = millis() - timeFallRise;
			if ( speedPressCounter++ == (EDGE_COUNTER - 1) ) {
				timeFall2Rise = millis() - timeFall2Rise;
			}
			break;
		default: break;
	}
	return state;
}
/************************************************************************/

/***************************** Edge analyzing ***************************/
static void INLINE analyze_state_fall(void) {
	if ( keyAction == action_idle ) {
		keyAction 	  = action_debounce;
		debounceTimer = DEBOUNCE_TIME;
	}
}
/************************************************************************/
/************************* Stable state analyzing ***********************/
static KEY_EVENT_t INLINE analyze_state_low(void) {
	KEY_EVENT_t keyEvent = NO_EVENT;

	switch ( keyAction ) {
		case action_debounce:
			if ( debounceTimer == 0 ) {
				keyAction 	  = action_check;
				debounceTimer = SHORT_MAX_TIME;
			}
			gpio_pin_IMPULSES( DEBUG_PORT0, DEBUG_PIN0, 1 );
			break;
		case action_check:
			if ( debounceTimer == 0 ) {
				keyAction 	  = action_repeat;
				debounceTimer = MEDIUM_MAX_TIME;
				repeatCounter = 0;
			}
			gpio_pin_IMPULSES( DEBUG_PORT0, DEBUG_PIN0, 2 );
			break;
		case action_repeat:
			if ( debounceTimer == 0 ) {
				debounceTimer = REPEAT_TIME;
				repeatCounter++;
				if ( repeatCounter == VLONG_TIME_CNT ) {
					keyEvent = VLONG_PRESS;
				} else {
					keyEvent = KEY_REPEAT;
				}
			}
			gpio_pin_IMPULSES( DEBUG_PORT0, DEBUG_PIN0, 3 );
			break;
		default:break;
	}
	return keyEvent;
}
/***************************** Edge analyzing ***************************/
static void INLINE analyze_state_rise(void) {
	if ( keyAction == action_check || keyAction == action_repeat ) {
		debounceTimer = DEBOUNCE_TIME;
	}
}
/************************************************************************/
static KEY_EVENT_t INLINE analyze_state_high(void) {
	KEY_EVENT_t keyEvent = NO_EVENT;

	switch ( keyAction ) {
		case action_check:
			if ( debounceTimer == 0 ) {
				if ( timeFallRise > SHORT_MIN_TIME ) {
					speedPressCounter = 0;
					keyEvent = SHORT_PRESS;
				} else 	{
					if ( speedPressCounter == EDGE_COUNTER ) {

						if ( timeFall2Rise < MULTI_PRESS_TIME ) {
							keyEvent = DOUBLE_PRESS;
						}
						speedPressCounter = 0;
					}
				}
				keyAction = action_idle;
			}
			gpio_pin_IMPULSES( DEBUG_PORT0, DEBUG_PIN0, 4 );
			break;
		case action_repeat:
			if ( debounceTimer == 0 ) {
				if ( repeatCounter == 0 ) {
					keyEvent = MEDIUM_PRESS;
				}
				speedPressCounter = 0;
				keyAction = action_idle;
			}
			gpio_pin_IMPULSES( DEBUG_PORT0, DEBUG_PIN0, 5 );
			break;
		default: break;
	}
	return keyEvent;
}
/************************************************************************/

/********************** Main machine state analyzing ********************/
static KEY_EVENT_t key_pressed( void ) {
	static  uint8_t  	lastState = keyRELEASED;
			KEY_EVENT_t keyEvent  = false;

	uint8_t pressKeyCheck = sw_get_keys_simple();

	if (pressKeyCheck) {
		keyboard.keyCoded = pressKeyCheck;
		pressKeyCheck = keyPRESSED;
	} else {
		pressKeyCheck = keyRELEASED;
	}

	uint8_t slope = analyze_slope( pressKeyCheck, lastState );
	switch ( slope ) {
		case stateFALL:
			analyze_state_fall();
			break;
		case stateLOW:
			keyEvent = analyze_state_low();
			break;
		case stateRISE:
			analyze_state_rise();
			break;
		case stateHIGH:
			keyEvent = analyze_state_high();
			break;
		default:
			break;
	}
	lastState = pressKeyCheck;
	if (keyboard.keyCoded)return keyEvent;
	else  		 		  return NO_EVENT;
}
/************************************************************************/

/************************ Public functions ******************************/
KEYB_t * keyboard_ptr(void) {
	return &keyboard;
}

// ---------------------- Main init function -----------------------------
void sw_keyboard_init(void) {
#ifdef BTN1_PORT
	gpio_pin_cfg( BTN1_PORT, BTN1_PIN, gpio_mode_in_PU );
#endif
#ifdef BTN2_PORT
	gpio_pin_cfg( BTN2_PORT, BTN2_PIN, gpio_mode_in_PU );
#endif
}
// -----------------------------------------------------------------------

void SW_KEYBOARD_EVENT( void ) {
	keyboard.keyEvent = key_pressed();

	if ( keyboard.keyEvent != NO_EVENT ) {
		if( KeysTab[ keyboard.keyCoded - 1 ].keyboard_callback ) {
			KeysTab[ keyboard.keyCoded - 1 ].keyboard_callback();
			machine_state_reset();
		}
	}
}
/************************************************************************/



#if TEST_KEYBOARD
void keyboard(void) {
	static int8_t counter1, counter2, counter3;
	if ( keyboard_ptr()->shortPRESS == true ) {
		sw_led_start_blinking( 2, 100 );
		TEXT_display_float( 0, 0,  ++counter1,  	&TextX );
		sw_ssd1306_display();
	} else
	if ( keyboard_ptr()->mediumPRESS == true ) {
		TEXT_display_float( 0, 16,  ++counter2,  	&TextY );
		sw_ssd1306_display();
	} else
	if ( keyboard_ptr()->doublePRESS == true ) {
		TEXT_display_float( 0, 32,  ++counter3,  	&TextZ );
		sw_ssd1306_display();
	} else
	if ( keyboard_ptr()->veryLongPress == true ) {
		counter3 = 0; counter2 = 0; counter1 = 0;
		TEXT_display_float( 0,  0,  counter1,  	&TextX );
		TEXT_display_float( 0, 16,  counter2,  	&TextY );
		TEXT_display_float( 0, 32,  counter3,  	&TextZ );
		sw_led_start_blinking( 3, 100 );
		sw_ssd1306_display();
	} else
	if ( keyboard_ptr()->keyREPEAT == true ) {
		sw_led_on();
		TEXT_display_float( 0, 0,  ++counter1,  	&TextX );
		sw_ssd1306_display();
	}
}
#endif





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

