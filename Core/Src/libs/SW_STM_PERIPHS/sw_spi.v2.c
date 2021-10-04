/*
 * sw_spi.c
 *
 *  Created on: 28.10.2018
 *      Author: simon
 */
#include "sw_mcu_conf.h"
#include "../SW_BOARD/sw_gpio.h"
#include "../SW_TIMERS/sw_soft_timers.h"

#include "sw_spi.v2.h"


#if defined SPI_DMA || defined SPI_IRQ
//--------------------- Global, static variables -------------------------------------
static 			dma_buff_t 		spiDMAbuffTX[ SPI_DMA_BUFF_SIZE_TX ];
volatile static spiDMA_status_t spiDmaStatus = SPI_DMA_IDLE;
//------------------------------- Setters, getters -----------------------------------
dma_buff_t * sw_spi_get_buff(void) { return spiDMAbuffTX; }
//------------------------------------------------------------------------------------
#endif

static const SPI_t * spi = &spi1;
//------------------------------------------------------------------------------------

//----------------------- Short, static macro-functions ------------------------------
#ifdef SOFT_NSS
static inline void NSS_LO(void) { gpio_pin_LO( SOFT_NSS_PORT, SOFT_NSS_PIN ); }
static inline void NSS_HI(void) { gpio_pin_HI( SOFT_NSS_PORT, SOFT_NSS_PIN ); }
#endif
//------------------------------------------------------------------------------------


/* 1. ********************************************************************************/
static INLINE void sw_spi_data_size( uint32_t dataSizeBits ) {
	if ( dataSizeBits < 4 ) return;				// Data size from 4 - 16 bit

	CLEAR_BIT( spi->SPI->CR1, SPI_CR1_SPE );

	uint32_t tmp = ( (uint32_t)dataSizeBits - 1 ) << SPI_CR2_DS_Pos;
	MODIFY_REG( spi->SPI->CR2, SPI_CR2_DS, tmp );

	SET_BIT( spi->SPI->CR1, SPI_CR1_SPE );
}
/*************************************************************************************/

/* 2. ********************************************************************************/
static INLINE void sw_spi_nss_mode( void ) {
#ifdef SOFT_NSS
	SET_BIT( spi->SPI->CR1,
			 SPI_CR1_SSM 						// Software slave management
			|SPI_CR1_SSI 						// Internal slave select
		   );
	gpio_pin_cfg( SOFT_NSS_PORT, SOFT_NSS_PIN, gpio_mode_output_PP_HS );	// SPI NSS SW management
	NSS_HI();
#else
	SET_BIT( spi->SPI->CR2, SPI_CR2_SSOE );		// SS Output Enable
	SET_BIT( spi->SPI->CR2, SPI_CR2_NSSP ); 	// NSS pulse management Enable
//	spi->SPI->CR1 |= SPI_CR1_SSI;
	gpio_pin_cfg( spi->nss_port, spi->nss_pin, spi->alternateFun );	// SPI NSS HW management
#endif
}
/*************************************************************************************/

/* 3. ********************************************************************************/
#ifdef SPI_DMA
static INLINE void spi_wait_until_busy(void) {
	while ( (spi->SPI->SR & SPI_SR_BSY ) == SPI_SR_BSY ) {}
}
/*************************************************************************************/

/* 4. ********************************************************************************/
static INLINE uint8_t spi_check_busy(void) {
	if ( (spi->SPI->SR & SPI_SR_BSY) == 0 &&
		  spiDmaStatus == SPI_DMA_BUSY ){ return 1; }
	else 								{ return 0; }
}
/*************************************************************************************/

/* 5. ********************************************************************************/
static INLINE void spi_clear_tc_flag(void) {
	while( (DMA1->ISR & SPI_DMATX_TC_FLAG) == 0 ) {}	//
	DMA1->IFCR = SPI_DMATX_TC_FLAG;			// DMA Channel Transfer Complete clear flag
}
/*************************************************************************************/

/* 6. ********************************************************************************/
static void spi_dma_init( void ) {
#ifdef STM32F3
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
#elif defined STM32L4
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
#endif
	SPI_DMATX_Channel->CPAR = (uint32_t)(uint8_t *)&spi->SPI->DR;	// SPI data register
	SPI_DMATX_Channel->CMAR	= (uint32_t)(uint8_t *)spiDMAbuffTX;

	SPI_DMATX_Channel->CCR |=
							 DMA_CCR_TCIE			// Transfer complete interrupt enable
							|DMA_CCR_MSIZE_BYTE		// Peripheral size
							|DMA_CCR_PSIZE_BYTE		// Memory size
//							|DMA_CCR_MINC			//
							|DMA_CCR_DIR_MEMTOPHER
						;
	NVIC_EnableIRQ( SPI_DMA_ChannelTX_IRQn );
}
/* 7. ********************************************************************************/
void sw_spi_dma_send( uint8_t * buffer, uint8_t buffSize, uint8_t bitSize ) {
	sw_spi_data_size( bitSize );						//

	SPI_DMATX_Channel->CNDTR = (uint32_t)buffSize;		//
	SPI_DMATX_Channel->CCR 	|= DMA_CCR_EN;				//

	spiDmaStatus = SPI_DMA_START;
}
#endif
/*************************************************************************************/

/* 8. ********************************************************************************/
void sw_spi_hw_init( const SPI_param_t * param ) {
	if (spi->SPI == SPI1)
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
//	if (spi->SPI == SPI2)
//		RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;

	gpio_pin_cfg( spi->sck_port,  spi->sck_pin,  spi->alternateFun );
	gpio_pin_cfg( spi->miso_port, spi->miso_pin, spi->alternateFun );
	gpio_pin_cfg( spi->mosi_port, spi->mosi_pin, spi->alternateFun );

// Default 8-bit data
	sw_spi_nss_mode();
#ifdef SPI_DMA
	spi_dma_init();
	spi->SPI->CR2 |= SPI_CR2_TXDMAEN;
#endif
	spi->SPI->CR1 |= SPI_CR1_MSTR									// Master Selection
			  	  | (spiFclk_32 << SPI_CR1_BR_Pos);
	if (param->msblsb == LSBFIRST) {
		spi->SPI->CR1 |= SPI_CR1_LSBFIRST;
	} else {
		spi->SPI->CR1 &= ~SPI_CR1_LSBFIRST;
	}
	if (param->cpol == CPOL_HIGH) {
		spi->SPI->CR1 |= SPI_CR1_CPOL;
	} else {
		spi->SPI->CR1 &= ~SPI_CR1_CPOL;
	}
	if (param->cpha == CPOL_HIGH) {
		spi->SPI->CR1 |= SPI_CR1_CPHA;
	} else {
		spi->SPI->CR1 &= ~SPI_CR1_CPHA;
	}
#ifdef SPI_IRQ
	spi->SPI->CR2 |= SPI_CR2_TXEIE;			// Tx buffer Empty Interrupt Enable
#endif
	spi->SPI->SR = 0;
	spi->SPI->CR1 |= SPI_CR1_SPE;
}
/*************************************************************************************/

/* 9. ********************************************************************************/
void sw_spi_hw_send_buff( void * buffer, uint8_t buffSize, uint32_t bitSize ) {
	sw_spi_data_size( bitSize );
#ifdef SOFT_NSS
	NSS_LO();
#endif
	for ( uint8_t i = 0; i < buffSize; i++ ) {
		if (bitSize<9)	*(uint8_t  *)&spi->SPI->DR = *( (uint8_t  *)buffer + i );
		else						  spi->SPI->DR = *( (uint16_t *)buffer + i );
		while ( (spi->SPI->SR&SPI_SR_TXE) != SPI_SR_TXE ) {}
	}
	while ( spi->SPI->SR & SPI_SR_BSY ) {}
#ifdef SOFT_NSS
	NSS_HI();
#endif
}
/*************************************************************************************/

/* 10. *******************************************************************************/
uint16_t sw_spi_write_read( uint16_t data, uint32_t bitSize ) {
	sw_spi_data_size( bitSize );
	uint16_t dataLoc = 0;
#ifdef SOFT_NSS
	NSS_LO();
#endif
	if ( bitSize < 9 ) *(uint8_t *)&spi->SPI->DR = data;
	else						  	spi->SPI->DR = data;
	while ( (spi->SPI->SR & SPI_SR_TXE) != SPI_SR_TXE ) {}

	if ( bitSize<9 )dataLoc = *(uint8_t  *)&spi->SPI->DR;
	else			dataLoc = 				spi->SPI->DR;
	while ( spi->SPI->SR & SPI_SR_BSY ) {}
#ifdef SOFT_NSS
	NSS_HI();
#endif
	return (uint16_t)dataLoc;
}
/*************************************************************************************/

/* 11. *******************************************************************************/
#ifdef SPI_DMA
void SPI_DMA_ChannelTX_IRQHandler(void) {
	spi_clear_tc_flag();
//	spi_wait_until_busy();
	SPI_DMATX_Channel->CCR	&= ~DMA_CCR_EN;			//
	spiDmaStatus 			 = SPI_DMA_BUSY;
}
/*************************************************************************************/

/* 12. ********************* Event and callback function *****************************/
static void ( *spiDMAended_event_callback )( void );
void register_spiDMAended_event_callback( void (*callback)( void ) ) {
	spiDMAended_event_callback = callback;
}
/*************************************************************************************/

/* 13. *******************************************************************************/
void SW_SPI_DMA_END_EVENT( void ) {
	if ( spiDmaStatus == SPI_DMA_BUSY ) {
		if ( spi_check_busy() == 1 )
			spiDmaStatus = SPI_DMA_END;
	}
	if ( spiDmaStatus == SPI_DMA_END && spiDMAended_event_callback ) {
		spiDMAended_event_callback();
		spiDmaStatus = SPI_DMA_IDLE;
	}
}
/*************************************************************************************/
#endif
