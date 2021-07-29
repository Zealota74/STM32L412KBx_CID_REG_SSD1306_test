/*
 * mk_paj7620.c
 *
 *  Created on: 19 maj 2021
 *      Author: Miros�aw Karda�
 *
 *		www.atnel.pl           www.sklep.atnel.pl
 *      Gesture Library for PAJ7620 (Reliable LIB)
 *      ver: 1.0a
 *
 *      - basic 9 gestures with gesture filtering feature
 *      - we can add more than 810 own custom gestures!
 *      - we can use independant circle-continous cw & ccw gesture (FINGER as ROTARY ENCODER)
 *      - we can use INIT-GESTURE with user defined timeout to start recognize other basic and custom gestures
 *      - (callback with the last three gestures)
 *      - work with TWI/I2C, soft I2C and USI I2C (using MK_I2C Library)
 *      - work with INTx and PCINTx interrupt on ATmega/ATtiny
 *      - easy library and custom gestures configuration
 */
#ifdef __AVR__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#endif
#include <stdio.h>

#include "mk_paj7620.h"

#ifdef __AVR__
#include "../MK_I2C/mk_i2c.h"
#else
#define PROGMEM				// Delete PROGMEM macro in STM32
#define ACK				1
#define NACK			0
#define pgm_read_byte	*
#include "sw_mcu_conf.h"
#include "../SW_BOARD/sw_gpio.h"
#include "../SW_TIMERS/sw_soft_timers.h"
#include "../sw_i2c_simple_v2.h"
#define _delay_ms	delay_ms
#define _delay_us	delay_us

#endif


#if USE_CIRCLE_CONTINOUS == 1
	static TCIRCLECONTINOUS 	circle_callback;
	static volatile uint16_t *  cc_tmr;
	static int 					cc_cnt;
	static uint8_t 				enable_cc;
#endif


#if USE_INIT_GESTURE == 1
	static TINITGESTURE_START	init_gs_start_callback;
	static TINITGESTURE_END 	init_gs_end_callback;
	static volatile uint16_t * 	init_gs_tmr;
	static uint16_t 			init_gs_tmr_value;
	static TGESTUREFILTER 		init_gf_user_filter;
	static TGESTUREFILTER 		init_gf_filter;
	static uint8_t 				init_gs_state;
#endif


static TGESTURE 			gesture_callback;
static volatile uint8_t		paj_int_flag;
static uint8_t 				second_gs, first_gs;
static uint16_t				gfilter;
static volatile uint16_t  * gs_tmr;
static uint8_t				clear_last_gs_flag;


const uint8_t initRegisterArray[][2] PROGMEM = {	// konfiguracja czujnika w trybie rozpoznawania gest�w

		// zaj�to�� FLASH cz�ci inicjalizacyjnej - 158 bajt�w !
		{0xEF,0x00},
		{0x37,0x07},
		{0x38,0x17},
		{0x39,0x06},
		{0x42,0x01},
		{0x46,0x2D},
		{0x47,0x0F},
		{0x48,0x3C},
		{0x49,0x00},
		{0x4A,0x1E},
		{0x4C,0x20},
		{0x51,0x10},
		{0x5E,0x10},
		{0x60,0x27},
		{0x80,0x42},
		{0x81,0x44},
		{0x82,0x04},
		{0x8B,0x01},
		{0x90,0x06},
		{0x95,0x0C},
		{0x96,0x0C},
		{0x97,0x05},
		{0x9A,0x14},
		{0x9C,0x3F},
		{0xA5,0x19},
		{0xCC,0x19},
		{0xCD,0x0B},
		{0xCE,0x13},
		{0xCF,0x64},
		{0xD0,0x21},
		{0xEF,0x01},
		{0x02,0x0F},
		{0x03,0x10},
		{0x04,0x02},
		{0x25,0x01},
		{0x27,0x39},
		{0x28,0x7F},
		{0x29,0x08},
		{0x3E,0xFF},
		{0x5E,0x3D},
		{0x65,0x96},
		{0x67,0x97},
		{0x69,0xCD},
		{0x6A,0x01},
		{0x6D,0x2C},
		{0x6E,0x01},
		{0x72,0x01},
		{0x73,0x35},
		{0x74,0x00},
		{0x77,0x01},

		{0x93,0x0F},
		{0x94,0x0F},

		{0xEF,0x00},
		{0x41,0x00},
		{0x42,0x00},
		{0xEF,0x00},
		{0x48,0x3C},
		{0x49,0x00},
		{0x51,0x10},
		{0x83,0x20},
		{0x9f,0xf9},
		{0xEF,0x01},
		{0x01,0x1E},
		{0x02,0x0F},
		{0x03,0x10},
		{0x04,0x02},
		{0x41,0x40},
		{0x43,0x30},
		{0x65,0x96},
		{0x66,0x00},
		{0x67,0x97},
		{0x68,0x01},
		{0x69,0xCD},
		{0x6A,0x01},
		{0x6b,0xb0},
		{0x6c,0x04},
		{0x6D,0x2C},
		{0x6E,0x01},
		{0x74,0x00},
		{0xEF,0x00},
		{0x41,0xFF},
		{0x42,0x01},

};





static void paj7620_write_reg( uint8_t addr, uint8_t cmd ) {
#ifdef __AVR__
	i2c_start();
	i2c_write( addr );
	i2c_write( cmd );
	i2c_stop();
#elif defined STM32F3 || defined STM32L4
	sw_i2c_write_reg( PAJ7620_ADDR, addr, cmd );
#endif
}

static uint8_t paj7620_read_reg( uint8_t addr, uint8_t len, uint8_t * buf ) {
#ifdef __AVR__
	i2c_start();
	i2c_write( PAJ7620_ADDR );
	i2c_write( addr );
	i2c_start();
	i2c_write( PAJ7620_ADDR + 1 );
	while (len--) *buf++ = i2c_read( len ? ACK : NACK );
	i2c_stop();
#elif defined STM32F3 || defined STM32L4
	sw_i2c_read_block( PAJ7620_ADDR, addr, len, buf );
#endif

	return 0;
}



static void paj7620_bank_select( uint8_t bank ) {
	paj7620_write_reg( PAJ7620_REGITER_BANK_SEL, PAJ7620_BANK0 );
}

static void paj_int_config( void ) {
#if defined STM32F3
	RCC->APB2ENR 		|= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] 	|= SYSCFG_EXTICR1_EXTI3_PB;	// PB[3] pin
	EXTI->FTSR 			|= EXTI_FTSR_TR3;			// Falling trigger event configuration bit of line 3
	EXTI->RTSR 			|= EXTI_RTSR_TR3;			// Rising trigger event configuration bit of line 3
	EXTI->IMR 			|= EXTI_IMR_MR3;			// Interrupt Mask on line 3
	NVIC_EnableIRQ( EXTI3_IRQn );
#elif defined STM32L4
	RCC->APB2ENR 		|= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] 	|= SYSCFG_EXTICR1_EXTI2_PB;	// PB[3] pin
	EXTI->FTSR1 		|= EXTI_FTSR1_FT2;			// Falling trigger event configuration bit of line 3
	EXTI->RTSR1 		|= EXTI_RTSR1_RT2;			// Rising trigger event configuration bit of line 3
	EXTI->IMR1 			|= EXTI_IMR1_IM2;			// Interrupt Mask on line 3
	NVIC_EnableIRQ( EXTI3_IRQn );

#else

	PAJ_INT_PORT |= PAJ_INT_PIN_BIT;			// podci�ganie pinu INTx lub PCINx do VCC

	_delay_ms(5);

#if PAJ_INT_TYPE == PAJ_IRQ_PCINT
	PAJ_INT_EN_REG |= (1<<PAJ_INT_EN_BIT);
	PAJ_INT_MASK_REG |= (1<<PAJ_INT_MASK_BIT);
#endif

#if PAJ_INT_TYPE == PAJ_IRQ_INTx
	#ifndef MCUCSR
		#if PAJ_INT_EDGE == 0  // zbocze opadaj�ce
//			PAJ_SET_FALLING_EDGE;
		#endif
		#if PAJ_INT_EDGE == 1  // zbocze narastaj�ce
			PAJ_SET_RISING_EDGE;
		#endif
		#if PAJ_INT_EDGE == 2  // dowolna zmiana zbocza
			PAJ_SET_ANY_EDGE;
		#endif
	#endif
	#ifdef MCUCSR
		#if PAJ_INT_NR == 2
			#if PAJ_INT_EDGE == 0	// zbocze opadaj�ce
				MCUCSR &= ~(1<<ISC2);
			#endif
			#if PAJ_INT_EDGE == 1	// zbocze narastaj�ce
				MCUCSR |= (1<<ISC2);
			#endif
		#else
			#if PAJ_INT_EDGE == 0  // zbocze opadaj�ce
				PAJ_SET_FALLING_EDGE;
			#endif
			#if PAJ_INT_EDGE == 1  // zbocze narastaj�ce
				PAJ_SET_RISING_EDGE;
			#endif
			#if PAJ_INT_EDGE == 2  // dowolna zmiana zbocza
				PAJ_SET_ANY_EDGE;
			#endif
		#endif
	#endif

//			PAJ_INT_EN_REG |= ( 1<<PAJ_INT_BIT0(PAJ_INT_NR) );
#endif
#endif
}





//............................ FUNKCJE dla u�ytkownika biblioteki .................................

/* inicjalizacja pracy czujnika gest�w */
uint8_t paj7620_init( TFPS fps ) {
	paj_int_config();

	//*** WAKE UP - START ****
	_delay_ms(10);

#ifdef __AVR__
	i2c_start();
	i2c_write( PAJ7620_ADDR );
	i2c_stop();

	_delay_ms(1);
	i2c_start();
  	uint8_t res = i2c_write( PAJ7620_ADDR );
  	i2c_stop();
#elif defined STM32F3 || defined STM32L4
	uint8_t res;
	if ( sw_i2c_IsDeviceReady( PAJ7620_ADDR, 2, 1 ) == I2C_Error ) {
		res = 0;
	} else {
		res = 1;
	}
#endif
  	if( !res ) return 0x01; // je�li nie PAJ odpowiada to zwr�� b��d 0x01
  	//*** wake up - end ****



	paj7620_bank_select( BANK0 );

	uint16_t 	chip_id = 0;
	uint8_t 	tmo 	= 255;
	while( chip_id != 0x7620 ) {
		paj7620_read_reg( PAJ7620_ADDR_PART_ID_LOW, 2, (uint8_t*)&chip_id );
		_delay_us(500);
		if( !tmo-- ) break;
	}

	if( !tmo ) return 0xff;

	paj7620_bank_select( BANK0 );

	for( uint8_t i=0; i < INIT_REG_ARRAY_SIZE; i++ ) {
		paj7620_write_reg( pgm_read_byte( &initRegisterArray[i][0] ), pgm_read_byte( &initRegisterArray[i][1] ) );
	}

	paj7620_bank_select( BANK1 );
	if( !fps ) paj7620_write_reg( 0x65, 0xB7 );		// 120 fps
	else paj7620_write_reg( 0x65, 0x12 );			// 240 fps

	paj7620_bank_select( BANK0 );  //gesture flage reg in Bank0

	// inicjalizacja wewnetrznych przerwa� w module PJA
	uint8_t dat;
	paj7620_read_reg( 0x43, 1, &dat );
	paj7620_read_reg( 0x44, 1, &dat );

#if USE_CIRCLE_CONTINOUS == 1
  	enable_cc = 1;
#endif
  return 0;
}

#if USE_CIRCLE_CONTINOUS == 1

	/* je�li chcemy programowo wy��czy� na jaki� czas niezale�ne zdarzenia ci�g�ego kr�cenia k�ka, lub je w��czy�
	 *
	 * 		Argumenty:
	 * 		0 - zablokowanie zdarze� od kr�cenia k�ek CW i CCW
	 * 		1 - odblokowanie zdarze� kr�cenia k�ek CW i CCW (domy�lnie odblokowane po inicjalizacji)
	 */
	void enable_gs_circle_continous_mode( uint8_t enable ) {
		if( enable ) enable_cc = 1;
		else enable_cc = 0;
	}

#endif




/* Rejestracja w�asnego callbacka wywo�ywanego po ka�dym rozpoznaniu jednego z podstawowych 9 gest�w
 *
 * 		Argumenty:
 * 		1. w�asny callback - w�asna funckja zwrotna
 * 		2. wska�nik na timer programowy o podstawie czasu 10ms
 */
void register_gesture_callback( TGESTURE callback, volatile uint16_t * gs_timer ) {
	gesture_callback = callback;
	gs_tmr 			 = gs_timer;
	gs_set_filter(0);
}


#if USE_CIRCLE_CONTINOUS == 1
	/* je�li chcemy korzysta� ze zdarzenia ci�g�ego kr�cenia k�ka w prawo lub w lewo. WYMAGANY oddzielny timer programowy
	 *
	 * 		Argumenty:
	 * 		1. w�asny callback - wywo�ywany niezale�nie od pojedynczych gest�w CW i CCW, kt�re dzia�aj� na przerwaniach
	 * 		2. wska�nik na timer programowy o podstawie czasu 10ms
	 * 		3. warto�� inicjuj�ca licznik, kt�ry jest p�niej przekazywany do callbacka (daj�cy funkcjonalno�� enkodera)
	 */
	void register_circle_continous_callback( TCIRCLECONTINOUS callback, volatile uint16_t * cc_timer, int cnt_start_val ) {
		circle_callback = callback;
		cc_tmr 			= cc_timer;
		cc_cnt 			= cnt_start_val;
	}
#endif


#if USE_INIT_GESTURE == 1

	/* inicjalizacja - tzw GESTU INICJUJ�CEGO prac� rozpoznawania kolejnych gest�w (rejestracja callbacka_
	 *
	 * 		Argumenty:
	 * 		1. w�asny callback - wywo�ywany w momencie wykryca gestu inicjuj�cego, mo�na pomin�� podaj�c warto�� 0 (NULL)
	 * 		2. filtr gestu np gf_wave (mo�na ��czy� kilka filtr�w)
	 * 		3. wska�nik na timer programowy o podstawie czasu 10ms
	 * 		4. czas w ms na Timeout po kt�rym ko�czy si� wykrywanie gest�w i nast�uje powr�t do stanu nas�uchiwania gestu inicjuj�cego, domy�lnie 2s
	 * 		   (je�li zostanie podana warto�� mniejsza ni� 800ms to zostanie zamieniona na 2000ms)
	 *
	 * 		(callback mo�e by� przydatny np do �wietlnej albo d�wi�kowej sygnalizacji startu rozpoznawania gest�w)
	 */
	void register_init_gs_start_callback( TINITGESTURE_START callback, TGESTUREFILTER gf_init, volatile uint16_t * init_tmr, uint16_t ms_interval ) {
		init_gs_start_callback 	= callback;
		init_gs_tmr 			= init_tmr;
		init_gf_filter 			= gf_init;
		init_gf_user_filter 	= init_gf_filter;
		gs_set_filter( init_gf_filter );
		init_gs_state = 0;
		if( (ms_interval) < 800 ) init_gs_tmr_value = 2000;
		else init_gs_tmr_value = ms_interval;
		*init_gs_tmr = init_gs_tmr_value/10;
	}

	/* rejestracja callbacka - wywo�ywanego w momencie gdy ko�czy si� czas rozpoznawania gest�w i nast�uje powr�t do stanu nas�uchiwania gestu inicjuj�cego
	 *
	 * 		Argumwenty:
	 * 		1. w�asny callback
	 *
	 * 		rejestracj� mo�na zupe�nie pomin�� je�li nie chcemy nic robi� w momencie ko�ca czasu rozpoznawania gest�w
	 * 		(mo�e by� przydatny np do �wietlnej albo d�wi�kowej sygnalizacji ko�ca rozpoznawania gest�w)
	 */
	void register_init_gs_end_callback( TINITGESTURE_END callback ) {
		init_gs_end_callback = callback;
	}
#endif




/* funkcja do zmiany filtr�w gest�w */
void gs_set_filter( TGESTUREFILTER filter ) {
	gfilter = filter;
}




/* g��wne zdarzenie (EVENT) kt�re nale�y zawsze wywo�ywa� w p�tli g��wnej programu */
void PAJ7620_EVENT( void ) {

	uint8_t 		error;//, res=0;
	uint16_t 		dat;
	static uint8_t 	res;

#if USE_CIRCLE_CONTINOUS == 1
	static int 		angle_acc, last_angle_acc;
	static uint8_t 	gs_cc, last_gs_cc=1, gs_block;
	int 			dif;

	if( enable_cc && cc_tmr && !(*cc_tmr) ) {

		if( gs_block ) gs_block--;
		if( gs_block == 1 ) {
			last_gs_cc = 0xFF; gs_cc = 0;
		}

		paj7620_read_reg( 0xc7, 2, (uint8_t*)&angle_acc );
		angle_acc &= 0x07FF;

		if( angle_acc ) {

			if( angle_acc != last_angle_acc ) {

				dif = angle_acc - last_angle_acc;
				if( angle_acc - last_angle_acc < -16 ) {
					gs_cc = gs_cw;
					if(circle_callback && last_gs_cc == gs_cc ) {
						cc_cnt++;
						circle_callback(gs_cc, &cc_cnt);
						gs_block = 25;
#if USE_INIT_GESTURE == 1
						if( init_gs_state ) *init_gs_tmr = init_gs_tmr_value;
#endif
					}
					last_angle_acc = angle_acc;
					last_gs_cc = gs_cc;
				}
				else if( dif > 16 ) {
					gs_cc = gs_ccw;
					if(circle_callback && last_gs_cc == gs_cc ) {
						cc_cnt--;
						circle_callback(gs_cc, &cc_cnt);
						gs_block = 25;
#if USE_INIT_GESTURE == 1
						if( init_gs_state ) *init_gs_tmr = init_gs_tmr_value;
#endif
					}
					last_angle_acc = angle_acc;
					last_gs_cc = gs_cc;
				}
			}
		}
		*cc_tmr = 2;
	}
#endif

#if USE_INIT_GESTURE == 1
	if( init_gs_state && init_gs_tmr && !(*init_gs_tmr) ) {
		init_gs_state 	= 0;
		*init_gs_tmr 	= init_gs_tmr_value/10;
		init_gf_filter 	= init_gf_user_filter;

		if( init_gs_end_callback ) init_gs_end_callback( &init_gf_filter );
		gs_set_filter( init_gf_filter );
		return;
	}
#endif



	if( gs_tmr && clear_last_gs_flag && !*gs_tmr ) {
		clear_last_gs_flag 	= 0;
		second_gs 			= 0;
		first_gs 			= 0;
	}

	if( paj_int_flag == 1 ) {
		paj_int_flag = 0;

		error = paj7620_read_reg( 0x43, 2, (uint8_t*)&dat );

		if( !error ) {
			res = 0;

			if( gfilter ) dat &= gfilter;

			if( dat ) {
				if( dat == 1 ) res = gs_right;
				else if( dat == 2 ) res = gs_left;
				else if( dat == 4 ) res = gs_up;
				else if( dat == 8 ) res = gs_down;
				else if( dat == 16 ) res = gs_forward;
				else if( dat == 32 ) res = gs_backward;
				else if( dat == 64 ) res = gs_cw;
				else if( dat == 128 ) res = gs_ccw;
				else if( dat == 256 ) res = gs_wave;
			}

			if( res && gesture_callback ) {
#if USE_INIT_GESTURE == 1

				*init_gs_tmr = init_gs_tmr_value/10;

				if( !init_gs_state ) {
					*init_gs_tmr = init_gs_tmr_value/10;
					init_gf_filter = gf_none;
					uint16_t tmr_tmp_val = init_gs_tmr_value;
					if( init_gs_start_callback ) init_gs_start_callback( &tmr_tmp_val, &init_gf_filter );
					if( tmr_tmp_val >= 800 ) *init_gs_tmr = tmr_tmp_val/10;
					gs_set_filter( init_gf_filter );
					first_gs = 0; second_gs = 0;
					init_gs_state = 1;
				} else {
#endif

#if USE_CIRCLE_CONTINOUS == 1
					if( !gs_block ) {
						gesture_callback( res, second_gs, first_gs );
						if( gs_tmr ) {
							first_gs 			= second_gs;
							second_gs 			= res;
							clear_last_gs_flag 	= 1;
							if( CUSTOM_GS_CLEAR_TIME >= 500 ) *gs_tmr = CUSTOM_GS_CLEAR_TIME/10;
							else *gs_tmr = 500;
						}
					} else {
						first_gs  = 0;
						second_gs = 0;
					}
#else
					gesture_callback( res, second_gs, first_gs );
					if( gs_tmr ) {
						first_gs = second_gs;
						second_gs = res;
						clear_last_gs_flag = 1;
						if( CUSTOM_GS_CLEAR_TIME >= 500 ) *gs_tmr = CUSTOM_GS_CLEAR_TIME/10;
						else *gs_tmr = 500;
					}
#endif

#if USE_INIT_GESTURE == 1
				}
#endif
			}

		}
	}
}



#ifdef __AVR__
#if ADD_ISR_NOBLOCK == 0
ISR( PAJ_IRQ_VECTOR ) {
#else
ISR( PAJ_IRQ_VECTOR, ISR_NOBLOCK ) {
#endif


	#if PAJ_INT_TYPE == PAJ_IRQ_INTx
		paj_int_flag = 1;
	#endif

	#if PAJ_INT_TYPE == PAJ_IRQ_PCINT
		if( !(PAJ_INT_PIN & PAJ_INT_PIN_BIT) ) {	// gdy zbocze opadaj�ce
			paj_int_flag = 1;
		}
	#endif
}
#else
INTERRUPT void EXTI2_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF2) {		// Pending bit for line 3
		EXTI->PR1 = EXTI_PR1_PIF2;
		paj_int_flag = 1;
	}
}
/*************************************************************************/

#endif
