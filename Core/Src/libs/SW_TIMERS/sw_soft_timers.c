/*
 * sw_soft_timers.c
 *
 *  Created on: 05.07.2017
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "../SW_BOARD/sw_gpio.h"

#include "sw_soft_timers.h"

#include "../SW_BOARD/sw_led_blink_debug.h"
#include "../keyboard.h"

// https://deepbluembedded.com/stm32-delay-microsecond-millisecond-utility-dwt-delay-timer-delay/
uint32_t DWT_Delay_Init(void) {
	/* Disable TRC */
	CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
	/* Enable TRC */
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

	/* Disable clock cycle counter */
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
	/* Enable  clock cycle counter */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

	/* Reset the clock cycle counter value */
	DWT->CYCCNT = 0;

	/* 3 NO OPERATION instructions */
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");

	/* Check if clock cycle counter has started */
	if (DWT->CYCCNT) {
		return 0; /*clock cycle counter started*/
	} else {
		return 1; /*clock cycle counter not started*/
	}
}

#define PRIGROUP_0G_16S	((const uint32_t) 0x07)
#define PRIGROUP_2G_8S	((const uint32_t) 0x06)
#define PRIGROUP_4G_4S	((const uint32_t) 0x05)
#define PRIGROUP_8G_2S	((const uint32_t) 0x04)
#define PRIGROUP_16G_0S	((const uint32_t) 0x03)

/************************ Interrupt priority*******************************/
void nvic_priority(void) {
	uint32_t prio;
	NVIC_SetPriorityGrouping( PRIGROUP_4G_4S );

	prio = NVIC_EncodePriority( PRIGROUP_4G_4S, 0, 0 ); // Draw the spectrum in RAM
	NVIC_SetPriority( SysTick_IRQn, prio );

	prio = NVIC_EncodePriority( PRIGROUP_4G_4S, 1, 0 ); // Update clock
	NVIC_SetPriority( EXTI3_IRQn, prio );

}
/************************************************************************/

/**** Inicjujemy SysTick pod timery programowe *****/
uint32_t sw_softTimers_init( uint32_t timeBase_ms, uint32_t type ) {
//	register_irDecoded_event_callback(sw_switch_mode);

//	if 		(type==MILI_SEC)	type = 1000;	// 1 ms
//	else if (type==MICRO_SEC) 	type = 1;		// 1 us
//	else return 0;								// błąd
//	SysTick_Config( SystemCoreClock/type/timeBase_ms );
	if ( type==MICRO_SEC ) {
		DWT_Delay_Init();
	}
	nvic_priority();
	return 1;									// ok
}
/***************************************************/


/******************************************************************************/
volatile uint16_t pressTimer, debounceTimer, repeatTimer;
volatile uint32_t riseTimer, fallTimer, peakTimer, fallAvgTimer, peakAvgTimer;
volatile uint16_t showMenuTimer;
volatile uint16_t softTimer2, softTimer5, softTimer3, whileTimer ;

volatile static uint16_t delayTimer;
volatile static uint32_t milis = 0;
void SysTick_Handler( void ) {
	milis++;
	if (pressTimer) 	pressTimer--;
	if (debounceTimer)	debounceTimer--;
	if (repeatTimer)	repeatTimer--;
	if (delayTimer)		delayTimer--;
	if (showMenuTimer) 	showMenuTimer--;
	if (softTimer2) 	softTimer2--;
	if (softTimer3) 	softTimer3--;
	if (softTimer5) 	softTimer5--;
	if (whileTimer) 	whileTimer--;

	sw_led_blinking();
//	SW_IR_DECODED_EVENT();
	SW_KEYBOARD_EVENT();

//	key_handler();
}
/******************************************************************************/
uint32_t millis(void) {
	return milis;
}
/*********************************************************/
/*********************************************************/
void delay_ms( uint16_t ms ) {
	delayTimer = ms;
	while ( delayTimer ) {}
}
/*********************************************************/

void delay_us( uint16_t us ) {
	  uint32_t au32_initial_ticks = DWT->CYCCNT;
	  uint32_t au32_ticks = ( uint32_t ) SystemCoreClock / 1000000;
	  us *= au32_ticks;
	  while ( (DWT->CYCCNT - au32_initial_ticks) < us - au32_ticks );
}
