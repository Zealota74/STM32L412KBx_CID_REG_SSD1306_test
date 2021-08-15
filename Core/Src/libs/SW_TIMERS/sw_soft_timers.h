/*
 * sw_soft_timers.h
 *
 *  Created on: 05.07.2017
 *      Author: simon
 */

#ifndef SW_SOFT_TIMERS_H_
#define SW_SOFT_TIMERS_H_

#define MILI_SEC	1
#define MICRO_SEC	2


extern uint32_t sw_softTimers_init( uint32_t timeBase_ms, uint32_t type );
extern void nvic_priority(void);


extern volatile uint16_t softTimer2, softTimer5, adcTimer;
extern volatile uint16_t pressTimer, debounceTimer, repeatTimer, adcCaptureTimer;
extern volatile uint32_t riseTimer, fallTimer, peakTimer, fallAvgTimer, peakAvgTimer;
extern volatile uint16_t showMenuTimer, softTimer3, whileTimer;

extern void delay_ms( uint16_t ms );
extern void delay_us( uint16_t delay);

unsigned long millis(void);


#endif /* SW_SOFT_TIMERS_H_ */
