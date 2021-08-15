/*
 * mpu6050.c
 *
 *  Created on: 08.10.2018
 *  	License: MIT
 *      Author: Mateusz Salamon
 *      Based on:
 *      	 - MPU-6000 and MPU-6050 Product Specification Revision 3.4
 *      	 - MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2
 *      	 - i2cdevlib by Jeff Rowberg on MIT license
 *      	 - SparkFun MPU-9250 Digital Motion Processor (DMP) Arduino Library on MIT License
 *
 *		www.msalamon.pl
 *		mateusz@msalamon.pl
 *
 *	Website: https://msalamon.pl/6-stopni-swobody-z-mpu6050-na-stm32/
 *	GitHub: https://github.com/lamik/MPU6050_STM32_HAL
 */

#include "sw_mcu_conf.h"
#include "SW_BOARD/sw_gpio.h"
#include "SW_TIMERS/sw_soft_timers.h"
#include "sw_i2c_simple_v2.h"


#include "sw_mpu6050.h"
#include "math.h"
#include <stdlib.h>

#define I2C_TIMEOUT 0

float Acc_Scale;
float Gyr_Scale;

static I2CSTATUS HAL_I2C_Mem_Read ( uint8_t devAddress, uint8_t regAddress, uint8_t regSize, uint8_t * buff, uint8_t len, uint8_t timeout ) {
	I2CSTATUS status;
	status = sw_i2c_read_bulk ( devAddress, regAddress, len, buff );
	delay_ms(2);
	return status;
}
static I2CSTATUS HAL_I2C_Mem_Write( uint8_t devAddress, uint8_t regAddress, uint8_t regSize, uint8_t * buff, uint8_t len, uint8_t timeout ) {
	I2CSTATUS status;
	status = sw_i2c_write_bulk( devAddress, regAddress, len, buff );
	delay_ms(2);
	return status;
}


//
// CONFIG
//
void MPU6050_SetDlpf(uint8_t Value) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_CONFIG, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= 0xF8;
	tmp |= (Value & 0x7);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_CONFIG, 1, &tmp, 1, I2C_TIMEOUT );
}

//
// PWR_MGMT_1
//
void MPU6050_DeviceReset(uint8_t Reset) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_PWR1_DEVICE_RESET_BIT);
	tmp |= ((Reset & 0x1) << MPU6050_PWR1_DEVICE_RESET_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetSleepEnabled(uint8_t Enable) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_PWR1_SLEEP_BIT);
	tmp |= ((Enable & 0x1) << MPU6050_PWR1_SLEEP_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetCycleEnabled(uint8_t Enable) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_PWR1_CYCLE_BIT);
	tmp |= ((Enable & 0x1) << MPU6050_PWR1_CYCLE_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetTemperatureSensorDisbled(uint8_t Disable) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_PWR1_TEMP_DIS_BIT);
	tmp |= ((Disable & 0x1) << MPU6050_PWR1_TEMP_DIS_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetClockSource(uint8_t Source) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= 0xF8;
	tmp |= (Source & 0x7);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 1, &tmp, 1, I2C_TIMEOUT );
}

//
//	PWR_MGMT_2
//
void MPU6050_SetLowPowerWakeUpFrequency(uint8_t Frequency) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= 0x3F;
	tmp |= (Frequency & 0x3) << MPU6050_PWR2_LP_WAKE_CTRL_BIT;
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_AccelerometerAxisStandby(uint8_t XA_Stby, uint8_t YA_Stby, uint8_t ZA_Stby) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= 0xC7;
	tmp |= ((XA_Stby & 0x1) << MPU6050_PWR2_STBY_XA_BIT)
			| ((YA_Stby & 0x1) << MPU6050_PWR2_STBY_YA_BIT)
			| ((ZA_Stby & 0x1) << MPU6050_PWR2_STBY_ZA_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_GyroscopeAxisStandby(uint8_t XG_Stby, uint8_t YG_Stby, uint8_t ZG_Stby) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= 0xF8;
	tmp |= ((XG_Stby & 0x1) << MPU6050_PWR2_STBY_XG_BIT)
			| ((YG_Stby & 0x1) << MPU6050_PWR2_STBY_YG_BIT)
			| ((ZG_Stby & 0x1) << MPU6050_PWR2_STBY_ZG_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 1, &tmp, 1, I2C_TIMEOUT );
}

//
//	Measurement scale configuration
//
void MPU6050_SetFullScaleGyroRange(uint8_t Range) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= 0xE7;
	tmp |= ((Range & 0x7) << 3);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, 1, &tmp, 1, I2C_TIMEOUT );

	switch (Range) {
	case MPU6050_GYRO_FS_250:
		Gyr_Scale = 0.007633;
		break;
	case MPU6050_GYRO_FS_500:
		Gyr_Scale = 0.015267;
		break;
	case MPU6050_GYRO_FS_1000:
		Gyr_Scale = 0.030487;
		break;
	case MPU6050_GYRO_FS_2000:
		Gyr_Scale = 0.060975;
		break;
	default:
		break;
	}
}

void MPU6050_SetFullScaleAccelRange(uint8_t Range) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= 0xE7;
	tmp |= ((Range & 0x7) << 3);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 1, &tmp, 1, I2C_TIMEOUT );

	switch (Range) {
	case MPU6050_ACCEL_FS_2:
		Acc_Scale = 0.000061;
		break;
	case MPU6050_ACCEL_FS_4:
		Acc_Scale = 0.000122;
		break;
	case MPU6050_ACCEL_FS_8:
		Acc_Scale = 0.000244;
		break;
	case MPU6050_ACCEL_FS_16:
		Acc_Scale = 0.0004882;
		break;
	default:
		break;
	}
}

//
// Reading data
//
int16_t MPU6050_GetTemperatureRAW(void) {
	uint8_t tmp[2];
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_TEMP_OUT_H, 1, tmp, 2, I2C_TIMEOUT );
	return (((int16_t) tmp[0]) << 8) | tmp[1];
}

float MPU6050_GetTemperatureCelsius(void) {
	int16_t temp;

	temp = MPU6050_GetTemperatureRAW();

	return (float) temp / 340 + 36.53;
}

int16_t MPU6050_GetAccelerationXRAW(void) {
	uint8_t tmp[2];
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, 1, tmp, 2, I2C_TIMEOUT );
	return (((int16_t) tmp[0]) << 8) | tmp[1];
}

int16_t MPU6050_GetAccelerationYRAW(void) {
	uint8_t tmp[2];
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_H, 1, tmp, 2, I2C_TIMEOUT );
	return (((int16_t) tmp[0]) << 8) | tmp[1];
}

int16_t MPU6050_GetAccelerationZRAW(void) {
	uint8_t tmp[2];
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_H, 1, tmp, 2, I2C_TIMEOUT );
	return (((int16_t) tmp[0]) << 8) | tmp[1];
}

void MPU6050_GetAccelerometerRAW(int16_t *x, int16_t *y, int16_t *z) {
	uint8_t tmp[6];
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, 1, tmp, 6, I2C_TIMEOUT );

	*x = (((int16_t) tmp[0]) << 8) | tmp[1];
	*y = (((int16_t) tmp[2]) << 8) | tmp[3];
	*z = (((int16_t) tmp[4]) << 8) | tmp[5];
}

void MPU6050_GetAccelerometerScaled(float *x, float *y, float *z) {
	int16_t tmp_x, tmp_y, tmp_z;
	MPU6050_GetAccelerometerRAW(&tmp_x, &tmp_y, &tmp_z);

	*x = (float) tmp_x * Acc_Scale;
	*y = (float) tmp_y * Acc_Scale;
	*z = (float) tmp_z * Acc_Scale;
}

int16_t MPU6050_GetRotationXRAW(void) {
	uint8_t tmp[2];
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, 1, tmp, 2, I2C_TIMEOUT );
	return (((int16_t) tmp[0]) << 8) | tmp[1];
}

int16_t MPU6050_GetRotationYRAW(void) {
	uint8_t tmp[2];
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H, 1, tmp, 2, I2C_TIMEOUT );
	return (((int16_t) tmp[0]) << 8) | tmp[1];
}

int16_t MPU6050_GetRotationZRAW(void) {
	uint8_t tmp[2];
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H, 1, tmp, 2, I2C_TIMEOUT );
	return (((int16_t) tmp[0]) << 8) | tmp[1];
}

void MPU6050_GetGyroscopeRAW(int16_t *x, int16_t *y, int16_t *z) {
	uint8_t tmp[6];
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, 1, tmp, 6, I2C_TIMEOUT );

	*x = (((int16_t) tmp[0]) << 8) | tmp[1];
	*y = (((int16_t) tmp[2]) << 8) | tmp[3];
	*z = (((int16_t) tmp[4]) << 8) | tmp[5];
}

void MPU6050_GetGyroscopeScaled(float *x, float *y, float *z) {
	int16_t tmp_x, tmp_y, tmp_z;

	MPU6050_GetGyroscopeRAW(&tmp_x, &tmp_y, &tmp_z);

	*x = (float) tmp_x * Gyr_Scale;
	*y = (float) tmp_y * Gyr_Scale;
	*z = (float) tmp_z * Gyr_Scale;
}

void MPU6050_GetRollPitch( float *Roll, float *Pitch, float *Yaw ) {
	float acc_x, acc_y, acc_z;
	MPU6050_GetAccelerometerScaled( &acc_x, &acc_y, &acc_z );

//	*Roll 	= 	atan2( acc_y, acc_z) * 180.0 / M_PI;
	*Roll 	= 	atan2( acc_y, sqrt( acc_x*acc_x + acc_z*acc_z ) ) * (180.0/M_PI);
//	*Pitch 	= -(atan2( acc_x, sqrt( acc_y*acc_y + acc_z*acc_z ) ) * 180.0 ) / M_PI;
	*Pitch 	=   atan2( acc_x, sqrt( acc_y*acc_y + acc_z*acc_z ) ) * (180.0/M_PI);
	*Yaw 	= 	atan2( acc_z, sqrt( acc_x*acc_x + acc_y*acc_y ) ) * (180.0/M_PI);
}

//
//	Setting INT pin
//
void MPU6050_SetInterruptMode(uint8_t Mode) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_INTCFG_INT_LEVEL_BIT);
	tmp |= ((Mode & 0x1) << MPU6050_INTCFG_INT_LEVEL_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetInterruptDrive(uint8_t Drive) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_INTCFG_INT_OPEN_BIT);
	tmp |= ((Drive & 0x1) << MPU6050_INTCFG_INT_OPEN_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetInterruptLatch(uint8_t Latch) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_INTCFG_INT_RD_CLEAR_BIT);
	tmp |= ((Latch & 0x1) << MPU6050_INTCFG_INT_RD_CLEAR_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetInterruptLatchClear(uint8_t Clear) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_INTCFG_LATCH_INT_EN_BIT);
	tmp |= ((Clear & 0x1) << MPU6050_INTCFG_LATCH_INT_EN_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetIntEnableRegister(uint8_t Value) {
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 1, &Value, 1, I2C_TIMEOUT );
}

void MPU6050_SetIntDataReadyEnabled(uint8_t Enable) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_INTERRUPT_DATA_RDY_BIT);
	tmp |= ((Enable & 0x1) << MPU6050_INTERRUPT_DATA_RDY_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 1, &tmp, 1, I2C_TIMEOUT );
}

uint8_t MPU6050_GetIntStatusRegister(void) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_INT_STATUS, 1, &tmp, 1, I2C_TIMEOUT );
	return tmp;
}

uint8_t MPU6050_GetDeviceID(void) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_WHO_AM_I, 1, &tmp, 1, I2C_TIMEOUT );
	return tmp << 1;
}

//
//	Motion functions - not included in documentation/register map
//
void MPU6050_SetDHPFMode(uint8_t Dhpf) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(0x07);
	tmp |= Dhpf & 0x7;
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 1, &tmp, 1, I2C_TIMEOUT );
}

uint8_t MPU6050_GetMotionStatusRegister(void) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_MOT_DETECT_STATUS, 1, &tmp, 1, I2C_TIMEOUT );
	return tmp;
}

void MPU6050_SetIntZeroMotionEnabled(uint8_t Enable) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_INTERRUPT_ZMOT_BIT);
	tmp |= ((Enable & 0x1) << MPU6050_INTERRUPT_ZMOT_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetIntMotionEnabled(uint8_t Enable) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_INTERRUPT_MOT_BIT);
	tmp |= ((Enable & 0x1) << MPU6050_INTERRUPT_MOT_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetIntFreeFallEnabled(uint8_t Enable) {
	uint8_t tmp;
	HAL_I2C_Mem_Read( MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 1, &tmp, 1, I2C_TIMEOUT );
	tmp &= ~(1 << MPU6050_INTERRUPT_FF_BIT);
	tmp |= ((Enable & 0x1) << MPU6050_INTERRUPT_FF_BIT);
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 1, &tmp, 1, I2C_TIMEOUT );
}

void MPU6050_SetMotionDetectionThreshold(uint8_t Threshold) {
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_MOT_THR, 1, &Threshold, 1, I2C_TIMEOUT );
}

void MPU6050_SetMotionDetectionDuration(uint8_t Duration) {
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_MOT_DUR, 1, &Duration, 1, I2C_TIMEOUT );
}

void MPU6050_SetZeroMotionDetectionThreshold(uint8_t Threshold) {
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_ZRMOT_THR, 1, &Threshold, 1, I2C_TIMEOUT );
}

void MPU6050_SetZeroMotionDetectionDuration(uint8_t Duration) {
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_ZRMOT_DUR, 1, &Duration, 1, I2C_TIMEOUT );
}

void MPU6050_SetFreeFallDetectionThreshold(uint8_t Threshold) {
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_FF_THR, 1, &Threshold, 1, I2C_TIMEOUT );
}

void MPU6050_SetFreeFallDetectionDuration(uint8_t Duration) {
	HAL_I2C_Mem_Write( MPU6050_ADDRESS, MPU6050_RA_FF_DUR, 1, &Duration, 1, I2C_TIMEOUT );
}

/********************************* GYRO *************************************/
int16_t MPU6050__getGyroOffsetX(void) {
	uint16_t value;
	sw_i2c_read_reg16( MPU6050_ADDRESS, MPU6050_REG_GYRO_XOFFS_H, &value );
	return (int16_t)value;
}
int16_t MPU6050__getGyroOffsetY(void) {
	uint16_t value;
	sw_i2c_read_reg16( MPU6050_ADDRESS, MPU6050_REG_GYRO_YOFFS_H, &value );
	return (int16_t)value;
}
int16_t MPU6050__getGyroOffsetZ(void) {
	uint16_t value;
	sw_i2c_read_reg16( MPU6050_ADDRESS, MPU6050_REG_GYRO_ZOFFS_H, &value );
	return (int16_t)value;
}
void MPU6050__setGyroOffsetX( int16_t offset ) {
	sw_i2c_write_reg16( MPU6050_ADDRESS, MPU6050_REG_GYRO_XOFFS_H, offset );
}
void MPU6050__setGyroOffsetY( int16_t offset ) {
	sw_i2c_write_reg16( MPU6050_ADDRESS, MPU6050_REG_GYRO_YOFFS_H, offset );
}
void MPU6050__setGyroOffsetZ( int16_t offset ) {
	sw_i2c_write_reg16( MPU6050_ADDRESS, MPU6050_REG_GYRO_ZOFFS_H, offset );
}
/*****************************************************************************/
/********************************* ACCEL *************************************/
int16_t MPU6050__getAccelOffsetX(void) {
	uint16_t value;
	sw_i2c_read_reg16( MPU6050_ADDRESS, MPU6050_REG_ACCEL_XOFFS_H, &value  );
	return (int16_t)value;
}
int16_t MPU6050__getAccelOffsetY(void) {
	uint16_t value;
	sw_i2c_read_reg16( MPU6050_ADDRESS, MPU6050_REG_ACCEL_YOFFS_H, &value  );
	return (int16_t)value;
}
int16_t MPU6050__getAccelOffsetZ(void) {
	uint16_t value;
	sw_i2c_read_reg16( MPU6050_ADDRESS, MPU6050_REG_ACCEL_ZOFFS_H, &value  );
	return (int16_t)value;
}
void MPU6050__setAccelOffsetX( int16_t offset ) {
	sw_i2c_write_reg16( MPU6050_ADDRESS, MPU6050_REG_ACCEL_XOFFS_H, offset );
}
void MPU6050__setAccelOffsetY( int16_t offset ) {
	sw_i2c_write_reg16( MPU6050_ADDRESS, MPU6050_REG_ACCEL_YOFFS_H, offset );
}
void MPU6050__setAccelOffsetZ( int16_t offset ) {
	sw_i2c_write_reg16( MPU6050_ADDRESS, MPU6050_REG_ACCEL_ZOFFS_H, offset );
}
/*****************************************************************************/



void sw_exti_init(void) {
	gpio_pin_cfg( PORTA, PA3, gpio_mode_in_floating );

#if defined STM32F3
	RCC->APB2ENR 		|= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] 	|= SYSCFG_EXTICR1_EXTI3_PB;	// PB[3] pin
	EXTI->FTSR 			|= EXTI_FTSR_TR3;			// Falling trigger event configuration bit of line 3
	EXTI->RTSR 			|= EXTI_RTSR_TR3;			// Rising trigger event configuration bit of line 3
	EXTI->IMR 			|= EXTI_IMR_MR3;			// Interrupt Mask on line 3
	NVIC_EnableIRQ( EXTI3_IRQn );
#elif defined STM32L4
	RCC->APB2ENR 		|= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] 	|= SYSCFG_EXTICR1_EXTI3_PA;	// PA[3] pin
	EXTI->FTSR1 		|= EXTI_FTSR1_FT3;			// Falling trigger event configuration bit of line 3
	EXTI->RTSR1 		|= EXTI_RTSR1_RT3;			// Rising trigger event configuration bit of line 3
	EXTI->IMR1 			|= EXTI_IMR1_IM3;			// Interrupt Mask on line 3
	NVIC_EnableIRQ( EXTI3_IRQn );
#endif
}

void EXTI3_IRQHandler(void) {
	if (EXTI->PR1 & EXTI_PR1_PIF3) {		// Pending bit for line 3
		EXTI->PR1 = EXTI_PR1_PIF3;

		if ( sw_is_BUSY_flag_ready() == true ) return;

		uint8_t interrupts = MPU6050_GetIntStatusRegister();
		MPU6050_GetMotionStatusRegister();

//		sprintf((char*) buffer, "Int status triggered: %X\n\n\r", interrupts);
//		UART1_SendString((char*) buffer);

		if (interrupts & (1 << MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) {			// Bit 4 (0x10)
//			sprintf((char*) buffer, "FIFO Overflow detected\n\r");
//			UART1_SendString((char*) buffer);
		}

		if (interrupts & (1 << MPU6050_INTERRUPT_MOT_BIT))	{				// Bit 6 (0x40)
//			sprintf((char*) buffer, "Motion detected\n\r");
//			UART1_SendString((char*) buffer);
		}

		if (interrupts & (1 << MPU6050_INTERRUPT_ZMOT_BIT))	{				// Bit 5 (0x20)
//			sprintf((char*) buffer, "Zero Motion detected\n\r");
//			UART1_SendString((char*) buffer);
		}

		if (interrupts & (1 << MPU6050_INTERRUPT_FF_BIT)) {					// Bit 7 (0x80)
//			sprintf((char*) buffer, "Freefall detected\n\r");
//			UART1_SendString((char*) buffer);
		}

	}
}
struct Vector {
	float XAxis;
	float YAxis;
	float ZAxis;
};
struct Vector	tg, dg, th; // ra, rg,
static float	actualThreshold;
static bool		useCalibrate = 0;
void MPU6050__calibrateGyro( uint8_t samples );
void MPU6050__setThreshold( uint8_t multiple );

// Set threshold value
void MPU6050__setThreshold( uint8_t multiple ) {
	if (multiple > 0) {
// If not calibrated, need calibrate
		if (!useCalibrate) {
			MPU6050__calibrateGyro( 50 );
		}
// Calculate threshold vectors
		tg.XAxis = th.XAxis * multiple;
		tg.YAxis = th.YAxis * multiple;
		tg.ZAxis = th.ZAxis * multiple;
	} else {
		// No threshold
		tg.XAxis = 0;
		tg.YAxis = 0;
		tg.ZAxis = 0;
	}
// Remember old threshold value
	actualThreshold = multiple;
}

// Get current threshold value
uint8_t MPU6050__getThreshold(void) {
	return actualThreshold;
}

// Calibrate algorithm
void MPU6050__calibrateGyro( uint8_t samples ) {
	// Set calibrate
	useCalibrate = true;

	// Reset values
	float sumX = 0;
	float sumY = 0;
	float sumZ = 0;
	float sigmaX = 0;
	float sigmaY = 0;
	float sigmaZ = 0;

	int16_t X = 0;
	int16_t Y = 0;
	int16_t Z = 0;

	// Read n-samples
	for ( uint8_t i = 0; i < samples; ++i ) {
		MPU6050_GetGyroscopeRAW( &X, &Y, &Z );
//		MPU6050__readRawGyro();
		sumX += X;
		sumY += Y;
		sumZ += Z;
		sigmaX += X * X;
		sigmaY += Y * Y;
		sigmaZ += Z * Z;
		delay_ms(5);
	}

	// Calculate delta vectors
	dg.XAxis = sumX / samples;
	dg.YAxis = sumY / samples;
	dg.ZAxis = sumZ / samples;

	// Calculate threshold vectors
	th.XAxis = sqrt( (sigmaX / 50) - (dg.XAxis * dg.XAxis) );
	th.YAxis = sqrt( (sigmaY / 50) - (dg.YAxis * dg.YAxis) );
	th.ZAxis = sqrt( (sigmaZ / 50) - (dg.ZAxis * dg.ZAxis) );

	// If already set threshold, recalculate threshold vectors
	if ( actualThreshold > 0 ) {
		MPU6050__setThreshold( actualThreshold );
	}
}


//
//	Initialization
//
void MPU6050_Init(void) {
	sw_exti_init();

	MPU6050_DeviceReset(1);
	MPU6050_SetSleepEnabled(0);
	MPU6050_SetClockSource(MPU6050_CLOCK_INTERNAL);
	MPU6050_SetDlpf(MPU6050_DLPF_BW_20);
	MPU6050_SetFullScaleGyroRange(MPU6050_GYRO_FS_250);
	MPU6050_SetFullScaleAccelRange(MPU6050_ACCEL_FS_2);

//	MPU6050__setGyroOffsetX(0);
//	MPU6050__setGyroOffsetY(0);
//	MPU6050__setGyroOffsetZ(0);
//	MPU6050__setAccelOffsetX(0);
//	MPU6050__setAccelOffsetY(0);
//	MPU6050__setAccelOffsetZ(0);
}

void sw_mpu6050_test_init(void) {
	MPU6050_Init();

	MPU6050_SetInterruptMode(MPU6050_INTMODE_ACTIVEHIGH);
	MPU6050_SetInterruptDrive(MPU6050_INTDRV_PUSHPULL);
	MPU6050_SetInterruptLatch(MPU6050_INTLATCH_WAITCLEAR);
	MPU6050_SetInterruptLatchClear(MPU6050_INTCLEAR_STATUSREAD);

	MPU6050_SetIntEnableRegister(0); // Disable all interrupts

	// Enable Motion interrupts
	MPU6050_SetDHPFMode(MPU6050_DHPF_5);

	MPU6050_SetIntMotionEnabled(1);
	MPU6050_SetIntZeroMotionEnabled(1);
	MPU6050_SetIntFreeFallEnabled(1);

	MPU6050_SetFreeFallDetectionDuration(2);
	MPU6050_SetFreeFallDetectionThreshold(5);

	MPU6050_SetMotionDetectionDuration(5);
	MPU6050_SetMotionDetectionThreshold(2);

	MPU6050_SetZeroMotionDetectionDuration(2);
	MPU6050_SetZeroMotionDetectionThreshold(4);

}



static int16_t ax, ay, az, gx, gy, gz;

static int mean_ax,mean_ay,mean_az,mean_gx,mean_gy,mean_gz;
//,state = 0;
static int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;

int buffersize 		= 1000;     //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
int acel_deadzone	= 8;     	//Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
int giro_deadzone	= 1;     	//Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)

void MPU6050_meansensors(void) {
	long i = 0, buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0, buff_gz = 0;

	while ( i < (buffersize + 101) ) {
		// read raw accel/gyro measurements from device
		MPU6050_GetGyroscopeRAW( &gx, &gy, &gz );
		MPU6050_GetAccelerometerRAW( &ax, &ay, &az );

		if ( i > 100 && i <= (buffersize + 100) ) { 	//First 100 measures are discarded
			buff_ax = buff_ax + ax;
			buff_ay = buff_ay + ay;
			buff_az = buff_az + az;
			buff_gx = buff_gx + gx;
			buff_gy = buff_gy + gy;
			buff_gz = buff_gz + gz;
		}
		if ( i == (buffersize + 100) ) {
			mean_ax = buff_ax / buffersize;
			mean_ay = buff_ay / buffersize;
			mean_az = buff_az / buffersize;
			mean_gx = buff_gx / buffersize;
			mean_gy = buff_gy / buffersize;
			mean_gz = buff_gz / buffersize;
		}
		i++;
		delay_ms(2);									// Needed so we don't get repeated measures
	}
}

void MPU6050_calibration(void) {
	ax_offset = -mean_ax / 8;
	ay_offset = -mean_ay / 8;
	az_offset = (16384 - mean_az) / 8;

	gx_offset = -mean_gx / 4;
	gy_offset = -mean_gy / 4;
	gz_offset = -mean_gz / 4;
	while (1) {
		int ready = 0;
		MPU6050__setAccelOffsetX( ax_offset );
		MPU6050__setAccelOffsetY( ay_offset );
		MPU6050__setAccelOffsetZ( az_offset );

		MPU6050__setGyroOffsetX( gx_offset );
		MPU6050__setGyroOffsetY( gy_offset );
		MPU6050__setGyroOffsetZ( gz_offset );

		MPU6050_meansensors();
//		Serial.println("...");

		if (abs(mean_ax) <= acel_deadzone)
			ready++;
		else
			ax_offset = ax_offset - mean_ax / acel_deadzone;

		if (abs(mean_ay) <= acel_deadzone)
			ready++;
		else
			ay_offset = ay_offset - mean_ay / acel_deadzone;

		if (abs(16384 - mean_az) <= acel_deadzone)
			ready++;
		else
			az_offset = az_offset + (16384 - mean_az) / acel_deadzone;

		if (abs(mean_gx) <= giro_deadzone)
			ready++;
		else
			gx_offset = gx_offset - mean_gx / (giro_deadzone + 1);

		if (abs(mean_gy) <= giro_deadzone)
			ready++;
		else
			gy_offset = gy_offset - mean_gy / (giro_deadzone + 1);

		if (abs(mean_gz) <= giro_deadzone)
			ready++;
		else
			gz_offset = gz_offset - mean_gz / (giro_deadzone + 1);

		if (ready == 6)
			break;
	}
}
