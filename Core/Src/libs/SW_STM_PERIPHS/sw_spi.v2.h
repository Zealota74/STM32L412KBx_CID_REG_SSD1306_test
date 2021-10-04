/*
 * sw_spi.h
 *
 *  Created on: 28.10.2018
 *      Author: simon
 */

#ifndef LIBS_SW_STM_PERIPHS_SW_SPI_V2_H_
#define LIBS_SW_STM_PERIPHS_SW_SPI_V2_H_

#include "sw_spi_irq_dma.h"

#define SPI_DMA
//#define SPI_IRQ
//

#if defined SPI_DMA || defined SPI_IRQ
#include "sw_spi_irq_dma.h"
#endif

#define SOFT_NSS				// { FALSE = hardware pin NSS, TRUE = software pin NSS }
enum {
	spi_8bit = 8, spi_12bit = 12, spi_16bit = 16
};

#ifdef SOFT_NSS
#	define SOFT_NSS_PORT	PORTA
#	define SOFT_NSS_PIN		PA2
#endif

typedef struct {
	uint8_t msblsb;
	uint8_t cpol;
	uint8_t cpha;
} SPI_param_t;

typedef struct {
	SPI_TypeDef * 	SPI;
	T_GPIO_MODE 	alternateFun;
	GPIO_TypeDef *  nss_port;
	GPIO_TypeDef *  sck_port;
	GPIO_TypeDef *  miso_port;
	GPIO_TypeDef *  mosi_port;
	T_GPIO_PIN 	    nss_pin, sck_pin, miso_pin, mosi_pin;
} SPI_t;
typedef struct {
	T_GPIO_MODE 	alternateFun;
	GPIO_TypeDef *  altPORT;
	T_GPIO_PIN 		altPIN;
} SPI_gpio_t;

#include "sw_spi_macros_v2.h"


#ifdef STM32F303xC
	static const SPI_t spi1 	= { SPI1, gpio_mode_AF5_PP_HS, PORTA, PORTA, PORTA, PORTA, PA4,  PA5,  PA6,  PA7  };
	static const SPI_t spi1ALT 	= { SPI1, gpio_mode_AF5_PP_HS, PORTA, PORTB, PORTB, PORTB, PA15, PB3,  PB4,  PB5  };
	static const SPI_t spi2 	= { SPI2, gpio_mode_AF5_PP_HS, PORTB, PORTB, PORTB, PORTB, PB12, PB13, PB14, PB15 };
	static const SPI_t spi3 	= { SPI3, gpio_mode_AF6_PP_HS, PORTA, PORTB, PORTB, PORTB, PA15, PB3,  PB4,  PB5  };
	static const SPI_gpio_t	spi3ALT_nss	= { gpio_mode_AF6_PP_HS, PORTA, PA4  };
#	ifdef 		 STM32F303RCTx
	static const SPI_t spi3ALT 	= { SPI3, gpio_mode_AF6_PP_MS, PORTA, PORTC, PORTC, PORTB, PA15, PC10, PC11, PC12 };
#	elif defined STM32F303CBTx
#	endif
#elif defined STM32F042x6
	static const SPI_t spi1 = { SPI1, gpio_mode_AF0_PP_MS, PORTA, PORTA, PORTA, PORTA, PA4,  PA5,  PA6,  PA7  };
#elif defined STM32F072xB
	static const SPI_t		spi1 		= { SPI1, gpio_mode_AF0_PP_MS, PORTA, PORTA, PORTA, PORTA, PA4,  PA5,  PA6,  PA7  };
	static const SPI_t 		spi1ALT 	= { SPI1, gpio_mode_AF0_PP_MS, PORTA, PORTB, PORTB, PORTB, PA15, PB3,  PB4,  PB5  };
	static const SPI_t 		spi2 		= { SPI2, gpio_mode_AF0_PP_MS, PORTB, PORTB, PORTB, PORTB, PB12, PB13, PB14, PB15 };
	static const SPI_gpio_t	spi1ALT_nss	= { gpio_mode_AF5_PP_MS, PORTB, PB9  };
	static const SPI_gpio_t	spi1ALT_sck	= { gpio_mode_AF5_PP_MS, PORTB, PB10 };
#elif defined STM32L4
	static const SPI_t spi1 = { SPI1, gpio_mode_AF5_PP_HS, PORTA, PORTA, PORTA, PORTA, PA4,  PA5,  PA6,  PA7  };
#endif


extern uint16_t		sw_spi_write_read( uint16_t data, uint32_t bitSize );
extern void 		sw_spi_hw_init( const SPI_param_t * param );
extern void 		sw_spi_hw_send_buff( void * buffer, uint8_t buffSize, uint32_t bitSize );
extern void 		sw_spi_write( uint8_t data, uint32_t bitSize );
extern uint8_t  	sw_spi_read( uint32_t bitSize );
extern dma_buff_t * sw_spi_get_buff(void);


#endif /* LIBS_SW_STM_PERIPHS_SW_SPI__V2_H_ */

