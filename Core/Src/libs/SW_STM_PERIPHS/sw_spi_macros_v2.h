/*
 * sw_spi_macros_v2.h
 *
 *  Created on: 20 cze 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_SW_STM_PERIPHS_SW_SPI_MACROS_V2_H_
#define SRC_LIBS_SW_STM_PERIPHS_SW_SPI_MACROS_V2_H_


#define MSBFIRST		0
#define LSBFIRST		1
#define CPOL_LOW		0
#define CPOL_HIGH		1
#define CPHA_LEADING	0
#define CPHA_TRAILING	1


#define SPI_CR2_DS_8BIT		(uint32_t)8
#define SPI_CR2_DS_16BIT	(uint32_t)16

#define DMA_CCR_PSIZE_BYTE 	( (uint32_t)0 )
#define DMA_CCR_PSIZE_HWORD	( (uint32_t)DMA_CCR_PSIZE_0 )
#define DMA_CCR_PSIZE_WORD	( (uint32_t)DMA_CCR_PSIZE_1 | DMA_CCR_PSIZE_0) )

#define DMA_CCR_MSIZE_BYTE 	( (uint32_t)0 )
#define DMA_CCR_MSIZE_HWORD	( (uint32_t)DMA_CCR_MSIZE_0 )
#define DMA_CCR_MSIZE_WORD	( (uint32_t)DMA_CCR_MSIZE_1 | DMA_CCR_MSIZE_0) )

#define DMA_CCR_DIR_PHERTOMEM	( (uint32_t)0 )
#define DMA_CCR_DIR_MEMTOPHER	( (uint32_t)DMA_CCR_DIR )

enum {	spiFclk_2,	spiFclk_4,	spiFclk_8,	 spiFclk_16,
		spiFclk_32,	spiFclk_64, spiFclk_128, spiFclk_256 };



#endif /* SRC_LIBS_SW_STM_PERIPHS_SW_SPI_MACROS_V2_H_ */
