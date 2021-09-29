/*
 * sw_i2c.h
 *
 *  Created on: 02.09.2018
 *      Author: simon
 */

#ifndef LIBS_SW_STM_PERIPHS_SW_I2C_H_
#define LIBS_SW_STM_PERIPHS_SW_I2C_H_


#ifdef	I2C_DMA
#define I2C_DMATX_Channel				DMA1_Channel6
#define I2C_DMATX_TC_FLAG				DMA_ISR_TCIF6		// Transfer Complete flag
#define I2C_DMA_ChannelTX_IRQn			DMA1_Channel6_IRQn
#define I2C_DMA_ChannelTX_IRQHandler	DMA1_Channel6_IRQHandler
#define I2C_DMA_BUFF_SIZE_TX			128
#define I2C_DMARX_Channel				DMA1_Channel7
#define I2C_DMARX_TC_FLAG				DMA_ISR_TCIF7		// Transfer Complete flag
#define I2C_DMA_ChannelRX_IRQn			DMA1_Channel7_IRQn
#define I2C_DMA_ChannelRX_IRQHandler	DMA1_Channel7_IRQHandler
#define I2C_DMA_BUFF_SIZE_RX			128
typedef int8_t 		spiDMA_status_t;
typedef uint8_t 	dma_buff_t;
#endif

#define I2C_ADDRESS_BYTE	1
#define I2C_ADDRESS_HWORD	2		// Half word - 2 bytes
#define I2C_ADDRESS_FWORD	3		// Word		 - 4 bytes




void DMA1_Channel6_IRQHandler( void );
void DMA1_Channel7_IRQHandler( void );
void I2C1_ER_IRQHandler( void );

void sw_i2c_dma_send_buff( uint8_t slaveAddr, uint8_t *buff, uint16_t sizeBuff );
void sw_i2c_dma_get_buff ( uint8_t slaveAddr, uint8_t *buff, uint16_t sizeBuff, uint8_t wordLength );
void sw_i2c_init( void );
I2CSTATUS sw_i2c_dma_test_slave( uint8_t slaveAddr, uint8_t testData );



#endif /* LIBS_SW_STM_PERIPHS_SW_I2C_H_ */
