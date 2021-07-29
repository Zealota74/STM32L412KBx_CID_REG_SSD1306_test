/*
 * sw_mcu_conf.h
 *
 *  Created on: 15.12.2017
 *      Author: simon
 */

#ifndef LIBS_SW_MCU_CONF_H_
#define LIBS_SW_MCU_CONF_H_

/********* Board type ********/
/*****************************/

/******* A tutaj wybieramy procesor *****************/
#ifdef STM32G0316_DISCO
#include "stm32g0xx.h"
#define BSRR_REG
#endif

#ifdef STM32F411RETx_spectrumRGB
#include "stm32f4xx.h"
#endif

#ifdef NUCLEO_STM32F303RE
#include "stm32f3xx.h"
#endif

#ifdef STM32F303CBTx_mini
#define BSRR_REG
#include "stm32f3xx.h"
#endif
#ifdef STM32L412KBx_nucleo
#define BSRR_REG
#include "stm32l4xx.h"
#endif
/****************************************************/

#include <stdint.h>

#define INLINE inline 	__attribute__((always_inline))
#define INTERRUPT 		__attribute__((interrupt))

#define CCMRAM 			__attribute__((section ("ccmram")))
#define OPTIMISE(x)		__attribute__((optimize(x)))

#ifndef bool
#include <stdbool.h>
#endif

#endif /* LIBS_SW_MCU_CONF_H_ */
