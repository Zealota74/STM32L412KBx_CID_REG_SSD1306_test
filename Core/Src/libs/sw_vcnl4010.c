/*
 * ds3231.c
 *
 *  Created on: 6 lis 2016
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "SW_BOARD/sw_gpio.h"

#include "SW_TIMERS/sw_soft_timers.h"

#include "sw_i2c_simple_v2.h"
#include "sw_vcnl4010.h"


static void sw_hardware_TWI_write_buf(uint8_t devAddr, uint8_t reg, uint16_t nBytes, uint8_t *pBuff ) {
	sw_i2c_write_bulk( devAddr, reg, nBytes, pBuff );
	delay_us(20);
};
static void sw_hardware_TWI_read_buf( uint8_t devAddr, uint8_t reg, uint16_t nBytes, uint8_t *pBuff ) {
	sw_i2c_read_bulk ( devAddr, reg, nBytes, pBuff  );
	delay_us(20);
}

static void vcnl4010_write_reg( uint8_t devReg, uint8_t data ) {
	sw_i2c_write_reg8( VCNL4010_I2C_ADDR, devReg, data );
	delay_us(20);
}
static void vcnl4010_read_reg( uint8_t devReg, uint8_t *data ) {
	sw_i2c_read_reg8( VCNL4010_I2C_ADDR, devReg, data );
	delay_us(20);
}

//volatile static uint8_t measure_ready_flag 		= 0;
volatile static uint8_t measureDataReadyFlag	= 0;

static T_RESULTS results;				// Kontener na pomiary

static void ( *measure_callback )( T_RESULTS *results );
void register_measure_callback( void (*callback)( T_RESULTS *results) ) {
	measure_callback = callback;
}

void vcnl4010_set_led_current( uint8_t current ) {
	if (current > 20)
		current = 20;
	vcnl4010_write_reg( VCNL4010_IR_LED_CURRENT_REG, current );
}

void vcnl4010_set_frequency( uint8_t freq ) {
	uint8_t temp_reg = 0;
	vcnl4010_read_reg( VCNL4010_MOD_TIMING, &temp_reg);

	temp_reg = temp_reg & ~((1<<3)|(1<<4));		// Maskujemy 4 i 3 bit rejestru ~(b00011000)
	freq = freq << 3;							// Ustawiamy freq na 3 i 4 bicie
	temp_reg = temp_reg | freq;					// Ustawiamy bity freq w rejestrze

	vcnl4010_write_reg( VCNL4010_MOD_TIMING, temp_reg );
}

static void vcnl_set_threshold( const uint8_t THRESHOLD, uint16_t threshold ) {
	uint8_t buf[2];
	buf[0] = (uint8_t)(threshold>>8)&0x00FF;
	buf[1] = (uint8_t)(threshold & 0x00FF);
	if ( !THRESHOLD )
		sw_hardware_TWI_write_buf( VCNL4010_I2C_ADDR, VCNL4010_THRESHOLD_RESULT_REG, 	 2, buf );
	else
		sw_hardware_TWI_write_buf( VCNL4010_I2C_ADDR, VCNL4010_THRESHOLD_RESULT_REG + 2, 2, buf );

}

uint8_t vcnl4010_init( void ) {
	uint8_t revision, control = INT_PROXI_READY_ENABLE;

	vcnl4010_read_reg( VCNL4010_PROD_ID_REVISION_REG, &revision);
	if ( (revision & 0xF0) != 0x20 ) {
		return 0;					// Error
	}
	vcnl4010_set_led_current( 20 );
	vcnl4010_set_frequency( VCNL4010_3M125 );
	vcnl_set_threshold( VCNL4010_THRESHOLD_LOW,  0xaabb );
	vcnl_set_threshold( VCNL4010_THRESHOLD_HIGH, 0xaabb );

	vcnl4010_write_reg( VCNL4010_INT_CONTROL_REG, control );

	RCC->APB2ENR 		|= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] 	|= SYSCFG_EXTICR1_EXTI3_PA;	// PB[3] pin
	EXTI->FTSR1 		|= EXTI_FTSR1_FT3;			// Falling trigger event configuration bit of line 3
	EXTI->RTSR1 		|= EXTI_RTSR1_RT3;			// Rising trigger event configuration bit of line 3
	EXTI->IMR1 			|= EXTI_IMR1_IM3;			// Interrupt Mask on line 3

	NVIC_EnableIRQ( EXTI3_IRQn );
	return 1;
}

void EXTI3_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF3) {				// Pending bit for line 3
		EXTI->PR1 = EXTI_PR1_PIF3;

	}
}

/************************* Funkcje pomocnicze ********************************
uint8_t vcnl4010_GET_register_value( const uint8_t REGISTER ) {
	uint8_t result;
	sw_hardware_TWI_read_buf( VCNL4010_I2C_ADDR, REGISTER, 1, &result);
	return result;
}
void vcnl4010_SET_register_value( const uint8_t REGISTER, uint8_t value ) {
	sw_hardware_TWI_write_buf( VCNL4010_I2C_ADDR, REGISTER, 1, &value);
}
*****************************************************************************/

/**************** USTAWIA i CZYTA flagi w rejestrach czujnika ************/
uint8_t vcnl4010_GET_measure_flag( const uint8_t REGISTER, const uint8_t FLAG ) {

	uint8_t result; //= vcnl4010_GET_register_value( REGISTER );
	vcnl4010_read_reg( REGISTER, &result);

	if ( result & FLAG )
		return 1;		// Flaga jest ustawiona na 1
	else
		return 0;		// Niestety flaga ustawiona na 0...
}
static void vcnl4010_SET_measure_flag( const uint8_t REGISTER, const uint8_t FLAG ) {
	uint8_t registry;

	// Pobieramy aktualną wartość rejestru REGISTER
	vcnl4010_read_reg( REGISTER, &registry);

	// Modyfikujemy (ustawiamy na 1) bit flagi
	registry = registry | FLAG;

	// Ładujemy zmodyfikowany rejestr
//	sw_hardware_TWI_write_buf( VCNL4010_I2C_ADDR, REGISTER, 1, &registry);
	vcnl4010_write_reg( REGISTER, registry );
}
/*************************************************************************/

static void vcnl4010_get_single_measure( T_RESULTS * results ) {
	static uint8_t measureStartFlag = 0;
	uint8_t i, buf[4];

	if ( measureStartFlag == 0) {
		vcnl4010_SET_measure_flag( VCNL4010_COMMAND_REG,
								   VCNL4010_SINGLE_AMBIL_START |
								   VCNL4010_SINGLE_PROXI_START);
		measureStartFlag = 1;
	}
	if ( vcnl4010_GET_measure_flag( VCNL4010_COMMAND_REG, VCNL4010_PROXI_DATAREADY_FLAG )
								&&( measureStartFlag == 1 ) ) {
		sw_hardware_TWI_read_buf( VCNL4010_I2C_ADDR, VCNL4010_AMBLIGHT_RESULT_REG, 4, buf);
		for (i=0; i<4; i++) {
			results->bytes[i] = buf[i];
		}
		sw_hardware_TWI_read_buf( VCNL4010_I2C_ADDR, VCNL4010_THRESHOLD_RESULT_REG, 4, buf);
		for (i=0; i<4; i++) {
			results->bytes[i+4] = buf[i];
		}
		measureStartFlag = 0;
		measureDataReadyFlag = 1;
	}
}

void SW_VCNL4010_MEASURE_EVENT( void ) {
	if (softTimer5==0) {
		vcnl4010_get_single_measure( &results );
		softTimer5=100;
	}

	if ( measure_callback && measureDataReadyFlag ) {
		if( measure_callback) {
			measure_callback( &results );
		}
		measureDataReadyFlag = 0;
	}
}

