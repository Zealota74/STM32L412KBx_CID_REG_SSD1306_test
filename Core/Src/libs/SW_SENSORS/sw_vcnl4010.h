/*
 * ds3231.h
 *
 *  Created on: 6 lis 2016
 *      Author: simon
 */

#ifndef VCNL4010_H_
#define VCNL4010_H_



#define VCNL4010_I2C_ADDR				0x26		// PDF,

#define VCNL4010_COMMAND_REG			0x80		// PDF, table 1, #0 Command Register
#define VCNL4010_PROD_ID_REVISION_REG	0x81		// PDF, table 2, #1 Product ID Revision Register
#define VCNL4010_PROXIMITY_RATE_REG		0x82		// PDF, table 3, #2 Rate of Proximity Measurement
#define VCNL4010_IR_LED_CURRENT_REG		0x83		// PDF, table 4, #3 LED Current Setting for Proximity Mode
#define VCNL4010_AMBLIGHT_PARAM_REG		0x84		// PDF, table 5, #4 Ambient Light Parameter Register
#define VCNL4010_AMBLIGHT_RESULT_REG	0x85		// PDF, table 6, #5 and #6 Ambient Light Result Register
#define VCNL4010_PROXIMITY_RESULT_REG	0x87		// PDF, table 8, #7 and #8 Proximity Measurement Result Register
#define VCNL4010_INT_CONTROL_REG		0x89		// PDF, table 10, #9 Interrupt Control Register
#define VCNL4010_PROXINITY_ADJUST		0x8A
#define VCNL4010_THRESHOLD_RESULT_REG	0x8A		// PDF, table 6, #5 and #6 Ambient Light Result Register
#define VCNL4010_INT_STATUS_REG			0x8E		// PDF, table 15, #14 Interrupt Status Register
#define VCNL4010_MOD_TIMING				0x8F

#define VCNL4010_AMBIL_DATAREADY_FLAG	(1<<6)
#define VCNL4010_PROXI_DATAREADY_FLAG	(1<<5)
#define VCNL4010_SINGLE_AMBIL_START		(1<<4)
#define VCNL4010_SINGLE_PROXI_START		(1<<3)

#define VCNL4010_AMBIL_ENABLE_FLAG		(1<<2)		// Third  bit of Command Register
#define VCNL4010_PROXI_ENABLE_FLAG		(1<<1)		// Second bit of Command Register
#define VCNL4010_SELFTIMED_ON_FLAG		(1<<0)		// First  bit of Command Register

#define	INT_PROXI_READY_ENABLE			(1<<3)
#define INT_AMBIL_READY_ENABLE			(1<<2)

#define VCNL4010_THRESHOLD_LOW			0
#define VCNL4010_THRESHOLD_HIGH			1


/************************** Unia przechowujaca dane dot. daty i czasu *********************/
typedef union {
	uint8_t bytes[8];
	struct {
		uint8_t ambientHigh;
		uint8_t ambientLow;
		uint8_t proximityHigh;
		uint8_t proximityLow;
	};
} T_RESULTS;
/******************************************************************************************/

typedef enum {
    VCNL4010_3M125   = 3,
    VCNL4010_1M5625  = 2,
    VCNL4010_781K25  = 1,
    VCNL4010_390K625 = 0,
} vcnl4010_freq;


void register_measure_callback(void (*callback)( T_RESULTS *results) );
void SW_VCNL4010_MEASURE_EVENT( void );

uint8_t vcnl4010_init(void);
extern void EXTI3_IRQHandler(void);

#endif /* VCNL4010_H_ */
