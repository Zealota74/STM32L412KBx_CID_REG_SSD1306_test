/*
 * sw_ADXL345.c
 *
 *  Created on: 2 sie 2021
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "../SW_BOARD/sw_gpio.h"
#include "../SW_TIMERS/sw_soft_timers.h"
#include "../SW_STM_PERIPHS/sw_i2c_base_v2.h"

#include "sw_ADXL345.h"


//static int constrain( int value, int min, int max ) {
//	if ( value < min ) return min;
//	if ( value > max ) return max;
//	return value;
//}

// Write byte to register
void ADXL345__writeRegister8( uint8_t reg, uint8_t value ) {
	sw_i2c_write_reg8( ADXL345_I2C_ADDR, reg, value );
}

// Read byte from register
uint8_t ADXL345__readRegister8( uint8_t reg ) {
	uint8_t value = 0;
	sw_i2c_read_reg8( ADXL345_I2C_ADDR, reg, &value );
	return value;
}

// Read word from register
int16_t ADXL345__readRegister16( uint8_t reg ) {
	int16_t value = 0;
	uint8_t buff[2];
	sw_i2c_read_bulk( ADXL345_I2C_ADDR, reg, 2, buff );
	value = (buff[1] << 8) | buff[0];
	return value;
}

static Vector_t xyzAxies;

void ADXL345__read_axies(void) {
	uint8_t buff[6];
	sw_i2c_read_bulk( ADXL345_I2C_ADDR, ADXL345_REG_DATAX0, 6, buff );

	xyzAxies.XAxis = (float)( (buff[1] << 8) | buff[0] );
	xyzAxies.XAxis = (float)( (buff[3] << 8) | buff[2] );
	xyzAxies.XAxis = (float)( (buff[5] << 8) | buff[4] );

}

void ADXL345__start_measure(void) {
	sw_i2c_write_reg8( ADXL345_I2C_ADDR, ADXL345_REG_POWER_CTL, ( 1<<3) ); // Third bit
}

I2CSTATUS ADXL345__init(void) {
	I2CSTATUS status = sw_i2c_slave_test( ADXL345_I2C_ADDR, 3, 100 );
	return status;
}
