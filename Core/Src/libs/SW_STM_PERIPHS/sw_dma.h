/*
 * sw_dma.h
 *
 *  Created on: 24.04.2018
 *      Author: simon
 */

#ifndef LIBS_SW_STM_PERIPHS_SW_DMA_H_
#define LIBS_SW_STM_PERIPHS_SW_DMA_H_


#define DMA_MSIZE_8BIT		(uint32_t)0
#define DMA_MSIZE_16BIT		(uint32_t)DMA_CCR_MSIZE_0
#define DMA_MSIZE_32BIT		(uint32_t)DMA_CCR_MSIZE_1
#define DMA_PSIZE_8BIT		(uint32_t)0
#define DMA_PSIZE_16BIT		(uint32_t)DMA_CCR_PSIZE_0
#define DMA_PSIZE_32BIT		(uint32_t)DMA_CCR_PSIZE_1

#define DMA_LOW_PRIORITY	(uint32_t)0
#define DMA_MED_PRIORITY	(uint32_t)DMA_CCR_PL_0
#define DMA_HIGH_PRIORITY	(uint32_t)DMA_CCR_PL_1
#define DMA_VHIGH_PRIORITY	(uint32_t)DMA_CCR_PL_1|DMA_CCR_PL_0

enum { Bits_8, Bits_16, Bits_32 };


void dma1_mem2mem( uint32_t * SrcBuff, uint32_t * DstBuff, uint32_t size, uint32_t bits );

void sw_dma_memset_init( uint32_t bits );
void sw_dma_memset_start( uint8_t * DstBuff, uint8_t pattern, uint32_t size );


#endif /* LIBS_SW_STM_PERIPHS_SW_DMA_H_ */
