/*
 * keyboard_keys.h
 *
 *  Created on: 24 sie 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_KEYBOARD_KEYS_H_
#define SRC_LIBS_KEYBOARD_KEYS_H_

#define KEYS_SIMPLE
//#define KEYS_MULTI
//#define KEYS_MATRIX


#ifdef 	KEYS_SIMPLE
#	define NUM_KEYS				2
#	define BTN1_PORT			PORTB
#	define BTN1_PIN				PB4
#	define BTN2_PORT			PORTB
#	define BTN2_PIN				PB5
#	define learn_key1_pressed()	gpio_pin_READ( BTN1_PORT, BTN1_PIN )
#	define learn_key2_pressed()	gpio_pin_READ( BTN2_PORT, BTN2_PIN )

	/********************************* Obsługa kilku klawiszy  *******************************/
	static inline uint8_t sw_get_keys_simple( void ) {
		if ( learn_key1_pressed() )
			return 1;									// Przycisk 1 wciśnięty
		else
		if ( learn_key2_pressed() )
			return 2;
		return 0;
	}
	/*****************************************************************************************/
#endif

#ifdef KEY_MULTI
#	define NUM_KEYS	10

#endif





#ifdef KEY_MATRIX
//TO DO
#endif

#endif /* SRC_LIBS_KEYBOARD_KEYS_H_ */
