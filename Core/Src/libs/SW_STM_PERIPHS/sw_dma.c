/*
 * sw_dma.c
 *
 *  Created on: 24.04.2018
 *      Author: simon
 */

#include "sw_mcu_conf.h"

void sw_dma_memset_init( uint32_t bits ) {
#ifdef STM32F3
	RCC->AHBENR |= RCC_AHBENR_DMA2EN;			// DMA1 clock enabled
#elif defined STM32L4
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;			// DMA1 clock enabled
#endif
	DMA2_Channel4->CCR &= ~DMA_CCR_EN;			// Clear CCR_EN bit - ready to CMAR,CPAR,CNDTR modification
	DMA2_Channel4->CCR = DMA_CCR_MEM2MEM		// Copy memory to memory
						 |DMA_CCR_MINC 	 		// Memory increment mode enabled
//						 |DMA_CCR_PINC 	 		// Peripheral increment mode enabled
//						 |DMA_CCR_DIR			// Data transfer direction. CMAR -> CPAR (Memory to Peripheral)
						 ;
	DMA2_Channel4->CCR &= ~(DMA_CCR_MSIZE | DMA_CCR_PSIZE);
	bits = ( bits << DMA_CCR_PSIZE_Pos );
	DMA2_Channel4->CCR |= bits;
	bits = ( bits << DMA_CCR_MSIZE_Pos );
	DMA2_Channel4->CCR |= bits;

}
void sw_dma_memset_start( uint8_t * DstBuff, uint8_t pattern, uint32_t size ) {
	// CNDTR, CMAR and CPAR allow modified when channel is OFF
//	DMA2->IFCR = DMA_IFCR_CTCIF4;								// Channel 4 Transfer Complete clear

	DMA2_Channel4->CCR &= ~DMA_CCR_EN;							// Clear CCR_EN bit - ready to CMAR,CPAR,CNDTR modification
	DMA2_Channel4->CNDTR = size;
	DMA2_Channel4->CMAR = (uint32_t)( DstBuff );		// Memory start address, 32-bit pointer
	DMA2_Channel4->CPAR = (uint32_t)( &pattern );		// Peripheral start address, 32-bit pointer
	DMA2_Channel4->CCR |= DMA_CCR_EN;							// DMA channel1 ON, start of copy

	while( (DMA2->ISR & DMA_ISR_TCIF4) == 0 ) {};				// Wait for Channel1 Transfer Complete
	DMA2->IFCR = DMA_IFCR_CTCIF4;								// Channel 4 Transfer Complete clear
}



void dma1_mem2mem( uint32_t * SrcBuff, uint32_t * DstBuff, uint32_t size, uint32_t bits ) {
	// Source buffer size MUST be <= destination buffer

//	RCC->AHBENR |= RCC_AHBENR_DMA2EN;			// DMA1 clock enabled

	// CNDTR, CMAR and CPAR allow modified when channel is OFF
	DMA2_Channel4->CCR &= ~DMA_CCR_EN;				// Clear CCR_EN bit - ready to CMAR,CPAR,CNDTR modification

	DMA2_Channel4->CMAR = (uint32_t)( (uint8_t *)SrcBuff );		// Memory start address, 32-bit pointer
	DMA2_Channel4->CPAR = (uint32_t)( (uint8_t *)DstBuff );		// Peripheral start address, 32-bit pointer
	DMA2_Channel4->CNDTR = size;

	DMA2_Channel4->CCR = DMA_CCR_MEM2MEM |		// Copy memory to memory
//						 DMA_CCR_MSIZE_0 |		// 0b00 - 8 bit
//						 DMA_CCR_MSIZE_1 |		// 0b01 - 16 bit
//						 DMA_CCR_PSIZE_0 |		// 0b10 - 32 bit
//						 DMA_CCR_PSIZE_1 |		//
//						 DMA_CCR_MINC | 		// Memory increment mode enabled
						 DMA_CCR_PINC | 		// Peripheral increment mode enabled
						 DMA_CCR_DIR;			// Data transfer direction. CMAR -> CPAR (Memory to Peripheral)

	DMA2_Channel4->CCR &= ~(DMA_CCR_MSIZE | DMA_CCR_PSIZE);
	bits = (bits << DMA_CCR_PSIZE_Pos);
	DMA2_Channel4->CCR |= bits;

	DMA2_Channel4->CCR |= DMA_CCR_EN;			// DMA channel1 ON, start of copy

	while( (DMA2->ISR & DMA_ISR_TCIF4) == 0 );	// Wait for Channel1 Transfer Complete
	DMA2->IFCR = DMA_IFCR_CTCIF4;				// Channel 4 Transfer Complete clear
}
