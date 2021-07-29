/*
 * rgb_matrix_pin_conf.h
 *
 *  Created on: 07.10.2017
 *      Author: simon
 */

#ifndef LIBS_SW_GRAPHICS_RGB_MATRIX_PIN_CONF_H_
#define LIBS_SW_GRAPHICS_RGB_MATRIX_PIN_CONF_H_


#define SSD1306_I2C
//#define SSD1306_SPI

#ifdef SSD1306_SPI
#	define SPI_NSS_PORT		PORTA
#	define SPI_NSS_PIN		PA4
#	define SPI_SCK_PORT		PORTA
#	define SPI_SCK_PIN		PA5
#	define SPI_DC_PORT		PORTB
#	define SPI_DC_PIN		PB0
#	define SPI_MOSI_PORT	PORTA
#	define SPI_MOSI_PIN		PA7
#	define RES_PORT			PORTB
#	define RES_PIN			PB1
#elif defined SSD1306_I2C
#	define I2C_SCL_PORT	PORTB
#	define I2C_SCL_PIN		PB6
#	define I2C_SDA_PORT	PORTB
#	define I2C_SDA_PIN		PB7
#	define SSD1306_ADDR		0x78
#endif


#endif /* LIBS_SW_GRAPHICS_RGB_MATRIX_PIN_CONF_H_ */
