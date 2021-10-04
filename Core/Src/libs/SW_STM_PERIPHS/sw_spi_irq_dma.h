/*
 * sw_spi_irq_dma.h
 *
 *  Created on: 2 paź 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_SW_STM_PERIPHS_SW_SPI_IRQ_DMA_H_
#define SRC_LIBS_SW_STM_PERIPHS_SW_SPI_IRQ_DMA_H_



enum { SPI_DMA_IDLE = 0, SPI_DMA_START = 1, SPI_DMA_BUSY = 2, SPI_DMA_END = 3 };


#define SPI_DMATX_Channel				DMA1_Channel3
#define SPI_DMATX_TC_FLAG				DMA_ISR_TCIF3				// Transfer Complete flag
#define SPI_DMA_ChannelTX_IRQn			DMA1_Channel3_IRQn
#define SPI_DMA_ChannelTX_IRQHandler	DMA1_Channel3_IRQHandler
#define SPI_DMA_BUFF_SIZE_TX			128
typedef int8_t 							spiDMA_status_t;
typedef uint8_t 						dma_buff_t;



//---------------------------------------------------------------------------------
extern void sw_spi_dma_send( uint8_t * buffer, uint8_t buffSize, uint8_t bitSize );
extern void register_spiDMAended_event_callback( void (*callback)( void ) );
extern void SW_SPI_DMA_END_EVENT(void);


#endif /* SRC_LIBS_SW_STM_PERIPHS_SW_SPI_IRQ_DMA_H_ */
