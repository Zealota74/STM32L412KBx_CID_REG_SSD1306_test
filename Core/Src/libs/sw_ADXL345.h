/*
 * sw_ADXL345.h
 *
 *  Created on: 2 sie 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_MK_GESTURE_PAJ7620_SW_ADXL345_H_
#define SRC_LIBS_MK_GESTURE_PAJ7620_SW_ADXL345_H_

#define ADXL345_I2C_ADDR			(0x1D << 1)	/* ASEL = 0 ->0x1D, ASEL = 1 -> 0x53 */
#define ADXL345_DEVICE_ID			(0xE5)

#ifdef ADXL354_355
#define ADXL345_DEVICE_ID			(0xAD)
#define ADXL345_MEMS_ID				(0x1D)
#define ADXL345_PART_ID				(0xED)

#define ADXL345_REG_DEVICE_MST		(0x01)
#define ADXL345_REG_PARTID			(0x02)
#define ADXL345_REG_REVID			(0x03)
#define ADXL345_REG_STATUS			(0x04)
#define ADXL345_REG_FIFO_ENTRIES	(0x05)
#define ADXL345_REG_TEMP2			(0x06)
#define ADXL345_REG_TEMP1			(0x07)
#endif

#define ADXL345_REG_DEVICE_AD		(0x00)
#define ADXL345_THRESH_TAP			(0x1D)
#define ADXL345_REG_OFSX			(0x1E)
#define ADXL345_REG_OFSY			(0x1F)
#define ADXL345_REG_OFSZ			(0x20)
#define ADXL345_REG_DUR				(0x21)
#define ADXL345_REG_LATENT			(0x22)
#define ADXL345_REG_WINDOW			(0x23)
#define ADXL345_REG_THRESH_ACT		(0x24)
#define ADXL345_REG_THRESH_INACT	(0x25)
#define ADXL345_REG_TIME_INACT		(0x26)
#define ADXL345_REG_ACT_INACT_CTL	(0x27)
#define ADXL345_REG_THRESH_FF		(0x28)
#define ADXL345_REG_TIME_FF			(0x29)
#define ADXL345_REG_TAP_AXES		(0x2A)
#define ADXL345_REG_ACT_TAP_STATUS	(0x2B)
#define ADXL345_REG_BW_RATE			(0x2C)

#define ADXL345_REG_POWER_CTL		(0x2D)

#define ADXL345_REG_INT_ENABLE		(0x2E)
#define ADXL345_REG_INT_MAP			(0x2F)
#define ADXL345_REG_INT_SOURCE		(0x30)
#define ADXL345_REG_DATA_FORMAT		(0x31)

#define ADXL345_REG_DATAX0			(0x32)
#define ADXL345_REG_DATAX1			(0x33)
#define ADXL345_REG_DATAY0			(0x34)
#define ADXL345_REG_DATAY1			(0x35)
#define ADXL345_REG_DATAZ0			(0x36)
#define ADXL345_REG_DATAZ1			(0x37)

#define ADXL345_REG_FIFO_CTL		(0x38)
#define ADXL345_REG_FIFO_STATUS		(0x39)

#define ADXL345_GRAVITY_SUN			273.95f
#define ADXL345_GRAVITY_EARTH		9.80665f
#define ADXL345_GRAVITY_MOON		1.622f
#define ADXL345_GRAVITY_MARS		3.69f
#define ADXL345_GRAVITY_NONE		1.00f

typedef enum {
	ADXL345_DATARATE_3200HZ = 0b1111,
	ADXL345_DATARATE_1600HZ = 0b1110,
	ADXL345_DATARATE_800HZ	= 0b1101,
	ADXL345_DATARATE_400HZ	= 0b1100,
	ADXL345_DATARATE_200HZ	= 0b1011,
	ADXL345_DATARATE_100HZ 	= 0b1010,
	ADXL345_DATARATE_50HZ 	= 0b1001,
	ADXL345_DATARATE_25HZ 	= 0b1000,
	ADXL345_DATARATE_12_5HZ = 0b0111,
	ADXL345_DATARATE_6_25HZ = 0b0110,
	ADXL345_DATARATE_3_13HZ = 0b0101,
	ADXL345_DATARATE_1_56HZ = 0b0100,
	ADXL345_DATARATE_0_78HZ = 0b0011,
	ADXL345_DATARATE_0_39HZ = 0b0010,
	ADXL345_DATARATE_0_20HZ = 0b0001,
	ADXL345_DATARATE_0_10HZ = 0b0000
} adxl345_dataRate_t;

typedef enum {
	ADXL345_INT2 = 0b01, ADXL345_INT1 = 0b00
} adxl345_int_t;

typedef enum {
	ADXL345_DATA_READY = 0x07,
	ADXL345_SINGLE_TAP = 0x06,
	ADXL345_DOUBLE_TAP = 0x05,
	ADXL345_ACTIVITY = 0x04,
	ADXL345_INACTIVITY = 0x03,
	ADXL345_FREE_FALL = 0x02,
	ADXL345_WATERMARK = 0x01,
	ADXL345_OVERRUN = 0x00
} adxl345_activity_t;

typedef enum {
	ADXL345_RANGE_16G = 0b11,
	ADXL345_RANGE_8G = 0b10,
	ADXL345_RANGE_4G = 0b01,
	ADXL345_RANGE_2G = 0b00
} adxl345_range_t;

typedef struct {
	float XAxis;
	float YAxis;
	float ZAxis;
} Vector_t;

struct Activites {
	bool isOverrun;
	bool isWatermark;
	bool isFreeFall;
	bool isInactivity;
	bool isActivity;
	bool isActivityOnX;
	bool isActivityOnY;
	bool isActivityOnZ;
	bool isDoubleTap;
	bool isTap;
	bool isTapOnX;
	bool isTapOnY;
	bool isTapOnZ;
	bool isDataReady;
};




void ADXL345__read_axies(void);

void ADXL345__start_measure(void);

I2CSTATUS ADXL345__init(void);



#endif /* SRC_LIBS_MK_GESTURE_PAJ7620_SW_ADXL345_H_ */
