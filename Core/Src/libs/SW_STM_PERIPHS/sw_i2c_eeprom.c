/*
 * sw_i2c_eeprom.c
 *
 *  Created on: 29.09.2018
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "../SW_BOARD/sw_gpio.h"

#include "sw_i2c_simple_v2.h"
#include "sw_i2c_dma.h"
#include "sw_i2c_eeprom.h"


static	T_I2C_EEPROM_FRAME i2cRxBuff;
static 	T_I2C_EEPROM_FRAME i2cTxBuff = {
	{0,0,1,2,3,4,5,6,7,8}
};


/*************************************************************************/
I2CSTATUS at24cxx_test( void ) {
	I2CSTATUS test = sw_i2c_dma_test_slave( AT24CXX_ADDRESS, 0 );

	if (test == I2C_Error || test == I2C_Nack)
		return I2C_Error;
	return I2C_Ok;
}
/*************************************************************************/

/*************************************************************************/
void at24cxx_write_byte( uint16_t address, uint8_t byte ) {
	i2cTxBuff.memAddress = ( (uint16_t)address<<8)|((uint16_t)address>>8);
	i2cTxBuff.data[0] = byte;
	sw_i2c_dma_send_buff( AT24CXX_ADDRESS, (uint8_t *)&i2cTxBuff,
						  AT24CXX_ONE_BYTE + I2C_ADDRESS_HWORD );
}
/*************************************************************************/
/*************************************************************************/
void at24cxx_write_page( uint16_t address ) {
	i2cTxBuff.memAddress = ( (uint16_t)address<<8)|((uint16_t)address>>8);
	sw_i2c_dma_send_buff( AT24CXX_ADDRESS, (uint8_t *)&i2cTxBuff,
						  AT24CXX_PAGE_SIZE + I2C_ADDRESS_HWORD );
}
/*************************************************************************/


/*************************************************************************/
uint8_t at24cxx_read_byte( uint16_t address ) {
	i2cRxBuff.memAddress = ( (uint16_t)address<<8)|((uint16_t)address>>8);

	sw_i2c_dma_get_buff( AT24CXX_ADDRESS, (uint8_t *)&i2cRxBuff,
						1, I2C_ADDRESS_HWORD );

	return i2cRxBuff.data[0];
}
/*************************************************************************/

/*************************************************************************/
void  at24cxx_read_page( uint16_t address ) {
	i2cRxBuff.memAddress = ( (uint16_t)address<<8)|((uint16_t)address>>8);

	sw_i2c_dma_get_buff( AT24CXX_ADDRESS, (uint8_t *)&i2cRxBuff,
						 AT24CXX_PAGE_SIZE, I2C_ADDRESS_HWORD );
}
/*************************************************************************/

