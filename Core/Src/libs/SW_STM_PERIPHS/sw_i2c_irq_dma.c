/*
 * sw_i2c.c
 *
 *  Created on: 02.09.2018
 *      Author: simon
 */
#include "sw_mcu_conf.h"

#include "../SW_BOARD/sw_gpio.h"

#include "sw_i2c_base_v2.h"
#include "sw_i2c_irq_dma.h"


/************************************ PRIVATE VARIABLES ******************************************/
static const I2C_t * hI2Cx;

static bool i2cAutoEnd 		= 1;
static bool i2cErrorOcurred = 0;

static bool i2cTransmitted 	= 0;
static bool i2cReceived 	= 0;

/*************************************************************************************************/

//----------------------------------- PRIVATE FUNCTIONS -----------------------------------------//
// 	I2C configured in master mode to transmit
static INLINE void sw_i2c_master_transmit_mode( const I2C_t *hI2Cx, uint8_t slaveAddr, uint8_t sizeBuff ) {
	sw_i2c_nBytes( sizeBuff );
	sw_i2c_set_7bitAddr( slaveAddr );
	sw_i2c_write_dir();
}
/*************************************************************************************************/

/*************************************************************************************************/
// 	I2C configured in master mode to receive code example
static INLINE void sw_i2c_master_receive_mode( const I2C_t *hI2Cx, uint8_t slaveAddr, uint8_t sizeBuff  ) {
	sw_i2c_nBytes( sizeBuff );
	sw_i2c_set_7bitAddr( slaveAddr );
	sw_i2c_read_dir();

	sw_i2c_autoend_on();
	i2cAutoEnd = 1;

	sw_i2c_start();
}
/*************************************************************************************************/

/***********************************************************************/
void I2C1_ER_IRQHandler( void ) {
	if (hI2Cx->I2C->ISR & I2C_ISR_ARLO ) {	// Arbitration lost
		hI2Cx->I2C->ICR = I2C_ICR_ARLOCF;
		hI2Cx->I2C->CR1 &= ~I2C_CR1_PE;		// I2C off
	}
	if (hI2Cx->I2C->ISR & I2C_ISR_BERR ) {	// Bus error
		hI2Cx->I2C->ICR = I2C_ICR_BERRCF;
		hI2Cx->I2C->CR1 &= ~I2C_CR1_PE;		// I2C off
	}
	if (hI2Cx->I2C->ISR & I2C_ISR_OVR ) {		// Overrun / Underrun
		hI2Cx->I2C->ICR = I2C_ICR_OVRCF;
		hI2Cx->I2C->CR1 &= ~I2C_CR1_PE;		// I2C off
	}
	i2cErrorOcurred = 1;
}
/***********************************************************************/

/***********************************************************************/
static INLINE void sw_i2c_dma_init( const I2C_t * hI2Cx ) {
#ifdef STM32F3
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
#elif defined STM32L4
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

#endif
	// DMA transmit config
	I2C_DMATX_Channel->CPAR 	= (uint32_t)( &hI2Cx->I2C->TXDR );
	I2C_DMATX_Channel->CCR =
					  DMA_CCR_DIR
					 |DMA_CCR_TCIE	// Transfer complete interrupt enable
					 |DMA_CCR_MINC	// Memory increment mode
					 ;
	NVIC_EnableIRQ( I2C_DMA_ChannelTX_IRQn );

	// DMA receive config
	I2C_DMARX_Channel->CPAR = (uint32_t)( &hI2Cx->I2C->RXDR );
	I2C_DMARX_Channel->CCR =
							 DMA_CCR_TCIE	// Transfer complete interrupt enable
							|DMA_CCR_MINC	// Memory increment mode
					;
	NVIC_EnableIRQ( I2C_DMA_ChannelRX_IRQn );
//	NVIC_EnableIRQ( DMA1_Channel4_5_6_7_IRQn );

	hI2Cx->I2C->CR1 |=
				 I2C_CR1_TXDMAEN		// DMA transmission requests enable
				|I2C_CR1_RXDMAEN		// DMA reception requests enable
				;
	hI2Cx = sw_i2c_get_handle();
}
/***********************************************************************/

//--------------------------- PUBLIC FUNCTIONS ------------------------//
/***************** I2C TX buffer transmitted ***************************/
void I2C_DMA_ChannelTX_IRQHandler( void ) {
	if ( DMA1->ISR & I2C_DMATX_TC_FLAG ) {
		DMA1->IFCR = DMA_IFCR_CTCIF6; // Channel 6 Transfer Complete clear

		I2C_DMATX_Channel->CCR &= ~DMA_CCR_EN;
		i2cTransmitted = 1;
	}
}
/***********************************************************************/

/******************** I2C RX buffer received ***************************/
void I2C_DMA_ChannelRX_IRQHandler( void ) {
	if ( DMA1->ISR & I2C_DMARX_TC_FLAG ) {
		DMA1->IFCR = DMA_IFCR_CTCIF7; // Channel 7 Transfer Complete clear

		I2C_DMARX_Channel->CCR &= ~DMA_CCR_EN;

		i2cReceived = 1;
	}
}
/************************************************************************/

/*********************************************************************************/
void sw_i2c_dma_send_buff ( uint8_t slaveAddr, uint8_t *buffPtr, uint16_t sizeBuff ) {
	if (i2cErrorOcurred) return;

	sw_i2c_master_transmit_mode( hI2Cx, slaveAddr, sizeBuff );
	if (i2cAutoEnd == 1)
		hI2Cx->I2C->CR2 |= I2C_CR2_AUTOEND;						// Auto STOP at the end of frame
	else
		hI2Cx->I2C->CR2 &= ~I2C_CR2_AUTOEND;						// No STOP at the end of frame

	I2C_DMATX_Channel->CMAR	 = (uint32_t)( (uint8_t *)buffPtr );
	I2C_DMATX_Channel->CNDTR = sizeBuff;

//	TX transfer start
	hI2Cx->I2C->CR2 |= ( sizeBuff << I2C_CR2_NBYTES_Pos );

	I2C_DMATX_Channel->CCR  |= DMA_CCR_EN;
	hI2Cx->I2C->CR2 			|= I2C_CR2_START;
}
/*********************************************************************************/

/*********************************************************************************/
void sw_i2c_dma_get_buff ( uint8_t slaveAddr, uint8_t *buffPtr, uint16_t sizeBuff, uint8_t wordLength ) {
	if (i2cErrorOcurred) return;

// Send address to slave -------------------
	i2cAutoEnd = 0;
	sw_i2c_dma_send_buff( slaveAddr, buffPtr, wordLength );
	i2cAutoEnd = 1;
// -----------------------------------------
	uint32_t delay = 10000;
	while( (hI2Cx->I2C->ISR & I2C_ISR_TC) == 0 ) {
		if (delay-- == 0 ) 	{
			i2cErrorOcurred = 1;
			return;
		}
	}
	I2C_DMARX_Channel->CNDTR = (uint32_t)sizeBuff;
	I2C_DMARX_Channel->CMAR	 = (uint32_t)( (uint8_t *)buffPtr + wordLength );
	I2C_DMARX_Channel->CCR  |= DMA_CCR_EN;

	sw_i2c_master_receive_mode( hI2Cx, slaveAddr, sizeBuff );
}
/*********************************************************************************/

/***********************************************************************/
I2CSTATUS sw_i2c_dma_test_slave( uint8_t slaveAddr, uint8_t testData ) {
	uint8_t test = 0x00;
	sw_i2c_dma_send_buff( slaveAddr, (uint8_t *)&test, 1 );

	uint32_t delay = 10000;										// For 72MHz only
	while( (hI2Cx->I2C->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY) {	// Transmit data register empty
			if (delay-- == 0 ) 	{
				hI2Cx->I2C->CR1 &= ~I2C_CR1_PE;					// I2C disable
				i2cErrorOcurred = 1;
				return I2C_Error;
			}
		}
	if ( (hI2Cx->I2C->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF)
		return I2C_Nack;
// Test wypadł pomyślnie
	hI2Cx->I2C->ICR = 0xFFFF;
	i2cErrorOcurred = 0;

	return I2C_Ok;
}
/***********************************************************************/









/***********************************************************************
static void (*i2cTX_event_callback )( uint8_t * );
static void (*i2cRX_event_callback )( void );
static void (*i2cOVF_event_callback)( void );
******************* Kod dotyczący funkcji zwrotnych *******************
void sk_register_I2cTX_event_callback( void (*callback)( uint8_t * )) {
	i2cTX_event_callback = callback;
}
void sk_register_I2cRX_event_callback( void (*callback)( void )) {
	i2cRX_event_callback = callback;
}
void sk_register_I2cOVF_event_callback( void (*callback)( void )) {
	i2cOVF_event_callback = callback;
}
void SW_I2C_EVENTS(void) {
	if ( i2cReceived && i2cTransmitted==0 ) {
		if ( i2cRX_event_callback )
			i2cRX_event_callback();

//		for (uint8_t i =0 ;i<7; i++ ) {
//			ShowDateTime.bytes[i] = 0;//sw_bcd2dec( i2cRxBuff.data[i] );
//		}

		i2cReceived = 0;
	}
	if ( i2cTransmitted && i2cReceived==0 ) {
//		if ( i2cTX_event_callback )
//			i2cTX_event_callback();
		i2cTransmitted = 0;
	}
}

void sk_register_I2cTX_event_callback( void (*callback)( uint8_t * ));
void sk_register_I2cRX_event_callback( void (*callback)( void ));
void sk_register_I2cOVF_event_callback( void (*callback)( void ));

void SW_I2C_EVENTS(void);

***********************************************************************/







