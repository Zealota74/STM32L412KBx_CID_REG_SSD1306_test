/*
 * sw_i2c_eeprom.h
 *
 *  Created on: 29.09.2018
 *      Author: simon
 */

#ifndef LIBS_SW_RTC_SW_I2C_EEPROM_H_
#define LIBS_SW_RTC_SW_I2C_EEPROM_H_



//#define AT24CXX_ADDRESS		0x50	// DS1307 tiny RTC I2C module
#define AT24CXX_ADDRESS			0x57	// DS3231 RTC I2C module
#define AT24CXX_PAGE_SIZE		32
#define AT24CXX_ONE_BYTE		1

typedef union {
	uint8_t buffer[ 32 + 2 ];
	struct {
		uint16_t memAddress;
		uint8_t  data[32];
	};
} T_I2C_EEPROM_FRAME;



void at24cxx_write_byte( uint16_t address, uint8_t byte );
void at24cxx_write_page( uint16_t address );
uint8_t at24cxx_read_byte( uint16_t address );
void at24cxx_read_page( uint16_t address );

I2CSTATUS at24cxx_test( void );

#endif /* LIBS_SW_RTC_SW_I2C_EEPROM_H_ */
