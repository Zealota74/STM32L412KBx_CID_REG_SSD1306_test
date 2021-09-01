/*
 * sw_rcc.c
 *
 *  Created on: Jul 19, 2021
 *      Author: simon
 */
#include "sw_mcu_conf.h"




/**********************************************************************/
static INLINE void rcc_FlashLatency_set( uint32_t latency ) {
	// Select flash latency
	MODIFY_REG( FLASH->ACR, FLASH_ACR_LATENCY, latency );
	//wait for latency set flag
	while ( (FLASH->ACR & FLASH_ACR_LATENCY) == 0 ) {}
}
/**********************************************************************/

/**********************************************************************/
static INLINE void pwr_VoltScale_sel(void) {
	// Set voltage scaling to range 1
	MODIFY_REG( PWR->CR1, PWR_CR1_VOS, PWR_CR1_VOS_0 );
	// Wait for voltage scaling flag
	while ( (PWR->SR2 & PWR_SR2_VOSF) ) {}
}
/**********************************************************************/

/**********************************************************************/
static INLINE void rcc_ClockSource_on( uint32_t clock ) {
	uint32_t checkClock, reg;
	switch ( clock ) {
		case RCC_CR_HSION:
			checkClock = RCC_CR_HSIRDY;
			reg = RCC_CR_CSSON | RCC_CR_HSION;
			break;
		case RCC_CR_HSEON:
			checkClock = RCC_CR_HSERDY;
			reg = RCC_CR_CSSON | RCC_CR_HSEBYP | RCC_CR_HSEON;
			break;
		default:
			break;
	}
	//turn on HSE external, HSE bypass and security
	SET_BIT( RCC->CR, reg ) ;
	//wait for the HSE to be ready
	while ( (RCC->CR & checkClock ) == 0 ) {}
}
/**********************************************************************/

/**********************************************************************/
static INLINE void rcc_PllMull_set(void) {
	uint16_t pllm = 1;			// 1, 2 - 8
	uint16_t plln = 10;			// 8, 9 - 86
	uint16_t pllr = 2;			// 2, 4, 6, 8,
	pllr = (uint8_t)pllr/2 - 1;
	// reset and configure pll mull and div settings, and PLL source
	MODIFY_REG( RCC->PLLCFGR,
				RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLR,
				 ((pllm - 1)	<<	RCC_PLLCFGR_PLLM_Pos )
				|( plln			<<	RCC_PLLCFGR_PLLN_Pos )
				|( pllr			<<	RCC_PLLCFGR_PLLR_Pos )
			  );
}
/**********************************************************************/

/**********************************************************************/
static INLINE void rcc_PllSourceMux_switch( uint32_t source ) {
	SET_BIT( RCC->PLLCFGR, source );
}
/**********************************************************************/

/**********************************************************************/
static INLINE void rcc_SystemClockMux_switch( uint32_t pllSource ) {
	uint32_t sourceCheck;
	switch ( pllSource ) {
		case RCC_CFGR_SW_PLL:
			sourceCheck = RCC_CFGR_SWS_PLL;
			break;
		case RCC_CFGR_SW_HSI:
			sourceCheck = RCC_CFGR_SWS_HSI;
			break;
		case RCC_CFGR_SW_HSE:
			sourceCheck = RCC_CFGR_SWS_HSE;
			break;
		default:
			break;
	}
	// Set PLL as system clock
	SET_BIT( RCC->CFGR, pllSource);
	while ( (RCC->CFGR & sourceCheck) == 0 ) {}
}
/**********************************************************************/

/**********************************************************************/
static INLINE void rcc_Pll_on(void) {
	//turn on PLL, wait for ready
	SET_BIT( RCC->CR, RCC_CR_PLLON);
	SET_BIT( RCC->PLLCFGR, RCC_PLLCFGR_PLLREN );
	// wait for pll to ready
	while (((RCC->CR) & RCC_CR_PLLRDY) == 0) {}
}
/**********************************************************************/

/**********************************************************************/
static INLINE void rcc_SystemClock_bus_presscaler( void ) {
	MODIFY_REG( RCC->CFGR,
				RCC_CFGR_PPRE1
			   |RCC_CFGR_PPRE2
			   |RCC_CFGR_HPRE,
			    RCC_CFGR_PPRE1_DIV1
			   |RCC_CFGR_PPRE2_DIV1
			   |RCC_CFGR_HPRE_DIV1
			 );
}
/**********************************************************************/

/**********************************************************************/
void SystemClock_Config(void) {
	rcc_FlashLatency_set( FLASH_ACR_LATENCY_4WS );
	pwr_VoltScale_sel();

	rcc_ClockSource_on( RCC_CR_HSION );

	rcc_PllMull_set();
	rcc_PllSourceMux_switch( RCC_PLLCFGR_PLLSRC_HSI );
	rcc_Pll_on();

	rcc_SystemClock_bus_presscaler();

	rcc_SystemClockMux_switch( RCC_CFGR_SW_PLL );

	SystemCoreClockUpdate();
}
/**********************************************************************/



/*
 * https://www.elektroda.pl/rtvforum/viewtopic.php?p=19577038#19577038
 */
/**
 * Entering STOP2 power save mode. In this mode all clocks except LSI and LSE are disabled. StaticRAM content
 * is preserved, optionally GPIO and few other peripherals can be kept power up depending on configuration
 */
void pwr_save_enter_stop2(void) {

	// clear previous low power mode selection
	PWR->CR1 &= (0xFFFFFFFF ^ PWR_CR1_LPMS_Msk);

	// select STOP2
	PWR->CR1 |= PWR_CR1_LPMS_STOP2;

	// enable write access to RTC registers by writing two magic words
//	RTC->WPR = 0xCA;
//	RTC->WPR = 0x53;
//
//	// save an information that STOP2 mode has been applied
//	RTC->BKP4R |= IN_STOP2_MODE;

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	DBGMCU->CR &= (0xFFFFFFFF ^ (DBGMCU_CR_DBG_SLEEP_Msk | DBGMCU_CR_DBG_STOP_Msk | DBGMCU_CR_DBG_STANDBY_Msk));

	asm("sev");
	asm("wfi");
}

void system_clock_configure_auto_wakeup_l4(uint16_t seconds) {

	// enable access to backup domain
	PWR->CR1 |= PWR_CR1_DBP;

	// enable write access to RTC registers by writing two magic words
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	// disable wakeup timer
	RTC->CR &= (0xFFFFFFFF ^ RTC_CR_WUTE);

	// wait for wakeup timer to disable
//	while((RTC->ISR & RTC_ISR_WUTWF) == 0);
	while((RTC->ICSR & RTC_ICSR_WUTWF) == 0);

	// clear wakeup flag
//	RTC->ISR &= (0xFFFFFFFF ^ RTC_ISR_WUTF_Msk);
	RTC->ICSR &= (0xFFFFFFFF ^ RTC_ICSR_WUTWF_Msk);

	// set auto wakeup timer
	RTC->WUTR = seconds;

	// start wakeup timer once again
	RTC->CR |= RTC_CR_WUTE;

	// enabling wakeup interrupt
	RTC->CR |= RTC_CR_WUTIE;

	// enable 20th EXTI Line (RTC wakeup timer)
	EXTI->IMR1 |= EXTI_IMR1_IM20;

	// set 20th EXTI line to rising trigger
	EXTI->RTSR1 |= EXTI_RTSR1_RT20;

	// by enabling this all pending interrupt will wake up cpu from low-power mode, even from those disabled in NVIC
	SCB->SCR |= SCB_SCR_SEVONPEND_Msk;

	// enable wakeup interrupt
	NVIC_EnableIRQ(RTC_WKUP_IRQn);
}


#ifdef STM32L471xx
void RTC_WKUP_IRQHandler(void) {

	// clear pending interrupt
	NVIC_ClearPendingIRQ(RTC_WKUP_IRQn);

	RTC->ISR &= (0xFFFFFFFF ^ RTC_ISR_WUTF_Msk);

	EXTI->PR1 |= EXTI_PR1_PIF20;

	system_clock_configure_l4();

	led_flip_led1_upper();

	led_flip_led2_bottom();

	led_control_led1_upper(true);
}
#endif
