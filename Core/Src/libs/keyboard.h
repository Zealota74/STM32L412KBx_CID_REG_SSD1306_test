/*
 * keyboard.h
 *
 *  Created on: 12 sie 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_KEYBOARD_H_
#define SRC_LIBS_KEYBOARD_H_

/**** Definicja struktury ******/
typedef struct {
	uint8_t pressType;
	uint8_t keysDecoded[1];				// Obecnie możliwość wykrycia tylko 1 przycisku
} T_KEYB;

/***********************************/
#define BTN1_PORT				PORTB
#define BTN1_PIN				PB4
#define learn_key1_pressed()	gpio_pin_READ( BTN1_PORT, BTN1_PIN )

/********************************* Obsługa kilku klawiszy  *******************************/
static inline uint8_t sw_get_single_key( void ) {
	if ( learn_key1_pressed() )
		return 1;									// Przycisk 1 wciśnięty
	return 0;										// Przyciski zwolnione
}
/*****************************************************************************************/
enum {
	IDLE=0, SHORT=1, LONG=2, REPEAT=3
};
enum {
	action_idle, action_debounce, action_check, action_react, action_repeat
};



/************************** Another ****************************/
extern volatile uint8_t StateKey;
void key_handler( void );
/***************************************************************/


extern T_KEYB * keyboard_ptr(void);
extern void register_keyboard_callback(void (*callback)( void ) );
extern void SW_KEYBOARD_EVENT( void );


#endif /* SRC_LIBS_KEYBOARD_H_ */
