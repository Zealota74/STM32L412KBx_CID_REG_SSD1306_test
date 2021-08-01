/*
 * sw_i2c_simple.h
 *
 *  Created on: Jul 17, 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_SW_I2C_SIMPLE_H_
#define SRC_LIBS_SW_I2C_SIMPLE_H_

/******************************* Definition *********************************/
#define DUMMY 						0
#define I2C_CR2_NBYTE_MAX			255

// I2C standard mode
#define I2C_TIMING_8MHz_100KHz		0x00201D2B
#define I2C_TIMING_24MHz_100KHz		0x00805C89
#define I2C_TIMING_48MHz_100KHz		0x10805E89
#define I2C_TIMING_64MHz_100KHz		0x60302730
#define I2C_TIMING_72MHz_100KHz		0x10C08DCF
#define I2C_TIMING_80MHz_100KHz		0x10909CEC

// I2C fast mode
#define I2C_TIMING_8MHz_400KHz		0x0010020A
#define I2C_TIMING_24MHz_400KHz		0x0040131E
#define I2C_TIMING_48MHz_400KHz		0x00901850
#define I2C_TIMING_64MHz_400KHz		0x00C0216C
#define I2C_TIMING_72MHz_400KHz		0x00E0257A
#define I2C_TIMING_80MHz_400KHz		0x00702991
// I2C fast mode plus
#define I2C_TIMING_48MHz_600KHz		0x00700838
#define I2C_TIMING_64MHz_600KHz		0x00900B4C
#define I2C_TIMING_72MHz_600KHz		0x00A00D56
#define I2C_TIMING_80MHz_600KHz		0x00303B3C

#define I2C_TIMING_48MHz_1000KHz	0x00700818
#define I2C_TIMING_64MHz_1000KHz	0x00900B22
#define I2C_TIMING_72MHz_1000KHz	0x00A00D26
#define I2C_TIMING_80MHz_1000KHz	0x00300F33

#define I2C_SLAVE_ADDR				0x0D

/***************************************************************************/
typedef struct {
	I2C_TypeDef * 	I2C;
	T_GPIO_MODE 	alternateFun;
	GPIO_TypeDef *  scl_port, * sda_port;
	T_GPIO_PIN 	    scl_pin, 	sda_pin;
} I2C_t;
typedef enum {
	I2C_Ok = 0, I2C_Error = 1, I2C_Nack = 2
} I2CSTATUS;
typedef enum {
	I2C_Ready = true, I2C_NotReady = false
} I2C_READY;

#if defined STM32F303xC || defined STM32L412xx
static const I2C_t i2c1 	= { I2C1, gpio_mode_AF4_OD_HS, PORTA, PORTA, PA15, PA14 };
static const I2C_t i2c1Alt1	= { I2C1, gpio_mode_AF4_OD_HS, PORTB, PORTB, PB6,  PB7 };
static const I2C_t i2c1Alt2	= { I2C1, gpio_mode_AF4_OD_HS, PORTB, PORTB, PB9,  PB8 };

static const I2C_t i2c2 	= { I2C2, gpio_mode_AF4_OD_HS, PORTA, PORTA, PA10, PA9  };
//static const I2C_t i2c2Alt 	= { I2C2, gpio_mode_AF4_OD_HS, PORTF, PORTF, PF0,  PF1 };
#else

#endif
static const I2C_t * hI2Cx = &i2c1Alt1;

/***************************************************************************/
static INLINE bool sw_is_TC_flag_ready(void) {
	if( hI2Cx->I2C->ISR & I2C_ISR_TC ) return true; else  return false;
}
static INLINE bool sw_is_TCR_flag_ready(void) {
	if( hI2Cx->I2C->ISR & I2C_ISR_TCR ) return true; else  return false;
}
static INLINE bool sw_is_TXIS_flag_ready(void) {
	if( hI2Cx->I2C->ISR & I2C_ISR_TXIS ) return true; else  return false;
}
static INLINE bool sw_is_NACK_flag_ready(void) {
	if( hI2Cx->I2C->ISR & I2C_ISR_NACKF ) return true; else  return false;
}
static INLINE bool sw_is_RXNE_flag_ready(void) {
	if( hI2Cx->I2C->ISR & I2C_ISR_RXNE ) return true; else  return false;
}
static INLINE bool sw_is_BUSY_flag_ready(void) {
	if( hI2Cx->I2C->ISR & I2C_ISR_BUSY ) return true; else  return false;
}

static INLINE void sw_i2c_set_7bitAddr( uint8_t devAddr ) {
	MODIFY_REG( hI2Cx->I2C->CR2, I2C_CR2_SADD, devAddr << I2C_CR2_SADD_Pos );
}


static INLINE I2CSTATUS sw_i2c_start( void ) {
	hI2Cx->I2C->CR2 |= I2C_CR2_START;
	return I2C_Ok;
}
static INLINE void 		sw_i2c_stop(void)  {
	hI2Cx->I2C->CR2 |= I2C_CR2_STOP;
	while ( (hI2Cx->I2C->ISR & I2C_ISR_STOPF) == 0) {}
//	hI2Cx->I2C->ICR = I2C_ICR_STOPCF;
}
static INLINE I2CSTATUS sw_i2c_write( uint8_t data ) {
	hI2Cx->I2C->TXDR = data;								// First write byte
	while ( ( hI2Cx->I2C->ISR & I2C_ISR_TXE ) == 0 ) { }	// then check the flag - buffer empty
	return I2C_Ok;
}
static INLINE uint8_t	sw_i2c_read( uint8_t dummy ) {
	while ( ( hI2Cx->I2C->ISR & I2C_ISR_RXNE ) == 0 ) {}	// then check the flag
	return ( hI2Cx->I2C->RXDR & 0xFF );
}
static INLINE void	sw_i2c_set_bitrate( uint32_t bitrate) {

}



extern void sw_i2c_simple_init(void);

extern I2CSTATUS sw_i2c_write_byte( uint8_t byte );
extern I2CSTATUS sw_i2c_read_byte ( uint8_t * byte );
extern I2CSTATUS sw_i2c_write_bulk( uint8_t devAddr, uint8_t regAddr, uint16_t nBytes, const uint8_t * pBuff );
extern I2CSTATUS sw_i2c_read_bulk ( uint8_t  devAddr, uint8_t regAddr, uint16_t nBytes, uint8_t * pBuff );
extern I2CSTATUS sw_i2c_write_reg ( uint8_t devAddr, uint8_t reg, uint8_t data );
extern I2CSTATUS sw_i2c_read_reg  ( uint8_t devAddr, uint8_t reg, uint8_t *data );
extern I2CSTATUS sw_i2c_IsDeviceReady( uint8_t devAddr, uint32_t trials, uint16_t delayMS );

extern I2CSTATUS sw_i2c_slave_test( uint8_t devAddr );

#ifdef I2C_TEST
extern void 	 sw_ds3231_test(void);
extern void 	 sw_ds3231_eeprom_test( void );
extern void sw_i2c_test_write(uint8_t devAddr);
#endif


#endif /* SRC_LIBS_SW_I2C_SIMPLE_H_ */
