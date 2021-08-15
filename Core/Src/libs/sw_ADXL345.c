/*
 * sw_ADXL345.c
 *
 *  Created on: 2 sie 2021
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "SW_BOARD/sw_gpio.h"
#include "SW_TIMERS/sw_soft_timers.h"
#include "sw_i2c_simple_v2.h"

#include "sw_ADXL345.h"

static struct Vector r;
static struct Vector n;
static struct Vector f;
static struct Activites a;

//static adxl345_range_t _range;

static int constrain( int value, int min, int max ) {
	if ( value < min ) return min;
	if ( value > max ) return max;
	return value;
}

// Write byte to register
void ADXL345__writeRegister8(uint8_t reg, uint8_t value) {
//	Wire.beginTransmission(ADXL345_ADDRESS);
//	Wire.send(reg);
//	Wire.send(value);
//	Wire.endTransmission();
	sw_i2c_write_reg8( ADXL345_ADDRESS, reg, value );
}

// Read byte to register
uint8_t ADXL345__fastRegister8(uint8_t reg) {
	uint8_t value = 0;
//	Wire.beginTransmission(ADXL345_ADDRESS);
//	Wire.send(reg);
//	Wire.endTransmission();
//
//	Wire.requestFrom(ADXL345_ADDRESS, 1);
//	value = Wire.receive();
//	Wire.endTransmission();
	sw_i2c_read_reg8( ADXL345_ADDRESS, reg, &value );
	return value;
}

// Read byte from register
uint8_t ADXL345__readRegister8(uint8_t reg) {
	uint8_t value = 0;
//	Wire.beginTransmission(ADXL345_ADDRESS);
//	Wire.send(reg);
//	Wire.endTransmission();
//
//	Wire.beginTransmission(ADXL345_ADDRESS);
//	Wire.requestFrom(ADXL345_ADDRESS, 1);
//	while (!Wire.available()) {};
//	value = Wire.receive();
//	Wire.endTransmission();

	sw_i2c_read_reg8( ADXL345_ADDRESS, reg, &value );
	return value;
}

// Read word from register
int16_t ADXL345__readRegister16(uint8_t reg) {
	int16_t value = 0;
//	Wire.beginTransmission(ADXL345_ADDRESS);
//	Wire.send(reg);
//	Wire.endTransmission();
//
//	Wire.beginTransmission(ADXL345_ADDRESS);
//	Wire.requestFrom(ADXL345_ADDRESS, 2);
//	while (!Wire.available()) {};
//	uint8_t vla = Wire.receive();
//	uint8_t vha = Wire.receive();
//	Wire.endTransmission();
//
//	value = vha << 8 | vla;
	uint8_t buff[2];
	sw_i2c_read_bulk( ADXL345_ADDRESS, reg, 2, buff );
	value = (buff[1] << 8) | buff[0];
	return value;
}

void ADXL345__writeRegisterBit(uint8_t reg, uint8_t pos, bool state) {
	uint8_t value;
	value = ADXL345__readRegister8(reg);

	if (state) {
		value |= (1 << pos);
	} else {
		value &= ~(1 << pos);
	}

	ADXL345__writeRegister8(reg, value);
}

bool ADXL345__readRegisterBit(uint8_t reg, uint8_t pos) {
	uint8_t value;
	value = ADXL345__readRegister8(reg);
	return ((value >> pos) & 1);
}



// Set Range
void ADXL345__setRange( adxl345_range_t range ) {
	// Get actual value register
	uint8_t value = ADXL345__readRegister8(ADXL345_REG_DATA_FORMAT);

	// Update the data rate
	// (&) 0b11110000 (0xF0 - Leave HSB)
	// (|) 0b0000xx?? (range - Set range)
	// (|) 0b00001000 (0x08 - Set Full Res)
	value &= 0xF0;
	value |= range;
	value |= 0x08;

	ADXL345__writeRegister8(ADXL345_REG_DATA_FORMAT, value);
}

// Get Range
adxl345_range_t ADXL345__getRange(void) {
	return (adxl345_range_t) (ADXL345__readRegister8(ADXL345_REG_DATA_FORMAT) & 0x03);
}

// Set Data Rate
void ADXL345__setDataRate(adxl345_dataRate_t dataRate) {
	ADXL345__writeRegister8(ADXL345_REG_BW_RATE, dataRate);
}

// Get Data Rate
adxl345_dataRate_t ADXL345__getDataRate(void) {
	return (adxl345_dataRate_t) (ADXL345__readRegister8(ADXL345_REG_BW_RATE) & 0x0F);
}




void ADXL345__clearSettings(void) {
	ADXL345__setRange(ADXL345_RANGE_2G);
	ADXL345__setDataRate(ADXL345_DATARATE_100HZ);

	ADXL345__writeRegister8(ADXL345_REG_THRESH_TAP, 0x00);
	ADXL345__writeRegister8(ADXL345_REG_DUR, 0x00);
	ADXL345__writeRegister8(ADXL345_REG_LATENT, 0x00);
	ADXL345__writeRegister8(ADXL345_REG_WINDOW, 0x00);
	ADXL345__writeRegister8(ADXL345_REG_THRESH_ACT, 0x00);
	ADXL345__writeRegister8(ADXL345_REG_THRESH_INACT, 0x00);
	ADXL345__writeRegister8(ADXL345_REG_TIME_INACT, 0x00);
	ADXL345__writeRegister8(ADXL345_REG_THRESH_FF, 0x00);
	ADXL345__writeRegister8(ADXL345_REG_TIME_FF, 0x00);

	uint8_t value;

	value = ADXL345__readRegister8(ADXL345_REG_ACT_INACT_CTL);
	value &= 0b10001000;
	ADXL345__writeRegister8(ADXL345_REG_ACT_INACT_CTL, value);

	value = ADXL345__readRegister8(ADXL345_REG_TAP_AXES);
	value &= 0b11111000;
	ADXL345__writeRegister8(ADXL345_REG_TAP_AXES, value);
}




bool ADXL345__begin() {
	f.XAxis = 0;
	f.YAxis = 0;
	f.ZAxis = 0;

//	Wire.begin();

	// Check ADXL345 REG DEVID
	if (ADXL345__fastRegister8(ADXL345_REG_DEVID) != 0xE5) {
		return false;
	}

	// Enable measurement mode (0b00001000)
	ADXL345__writeRegister8(ADXL345_REG_POWER_CTL, 0x08);

	ADXL345__clearSettings();

	return true;
}

// Low Pass Filter
struct Vector ADXL345__lowPassFilter(struct Vector vector, float alpha) {
	f.XAxis = vector.XAxis * alpha + (f.XAxis * (1.0 - alpha));
	f.YAxis = vector.YAxis * alpha + (f.YAxis * (1.0 - alpha));
	f.ZAxis = vector.ZAxis * alpha + (f.ZAxis * (1.0 - alpha));
	return f;
}

// Read raw values
struct Vector ADXL345__readRaw(void) {
	r.XAxis = ADXL345__readRegister16(ADXL345_REG_DATAX0);
	r.YAxis = ADXL345__readRegister16(ADXL345_REG_DATAY0);
	r.ZAxis = ADXL345__readRegister16(ADXL345_REG_DATAZ0);
	return r;
}

// Read normalized values
struct Vector ADXL345__readNormalize(float gravityFactor) {
	ADXL345__readRaw();

	// (4 mg/LSB scale factor in Full Res) * gravity factor
	n.XAxis = r.XAxis * 0.004 * gravityFactor;
	n.YAxis = r.YAxis * 0.004 * gravityFactor;
	n.ZAxis = r.ZAxis * 0.004 * gravityFactor;

	return n;
}

// Read scaled values
struct Vector ADXL345__readScaled(void) {
	ADXL345__readRaw();

	// (4 mg/LSB scale factor in Full Res)
	n.XAxis = r.XAxis * 0.004;
	n.YAxis = r.YAxis * 0.004;
	n.ZAxis = r.ZAxis * 0.004;

	return n;
}

// Set Tap Threshold (62.5mg / LSB)
void ADXL345__setTapThreshold(float threshold) {
	uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
	ADXL345__writeRegister8(ADXL345_REG_THRESH_TAP, scaled);
}

// Get Tap Threshold (62.5mg / LSB)
float ADXL345__getTapThreshold(void) {
	return ADXL345__readRegister8(ADXL345_REG_THRESH_TAP) * 0.0625f;
}

// Set Tap Duration (625us / LSB)
void ADXL345__setTapDuration(float duration) {
	uint8_t scaled = constrain(duration / 0.000625f, 0, 255);
	ADXL345__writeRegister8(ADXL345_REG_DUR, scaled);
}

// Get Tap Duration (625us / LSB)
float ADXL345__getTapDuration(void) {
	return ADXL345__readRegister8(ADXL345_REG_DUR) * 0.000625f;
}

// Set Double Tap Latency (1.25ms / LSB)
void ADXL345__setDoubleTapLatency(float latency) {
	uint8_t scaled = constrain(latency / 0.00125f, 0, 255);
	ADXL345__writeRegister8(ADXL345_REG_LATENT, scaled);
}

// Get Double Tap Latency (1.25ms / LSB)
float ADXL345__getDoubleTapLatency() {
	return ADXL345__readRegister8(ADXL345_REG_LATENT) * 0.00125f;
}

// Set Double Tap Window (1.25ms / LSB)
void ADXL345__setDoubleTapWindow(float window) {
	uint8_t scaled = constrain(window / 0.00125f, 0, 255);
	ADXL345__writeRegister8(ADXL345_REG_WINDOW, scaled);
}

// Get Double Tap Window (1.25ms / LSB)
float ADXL345__getDoubleTapWindow(void) {
	return ADXL345__readRegister8(ADXL345_REG_WINDOW) * 0.00125f;
}

// Set Activity Threshold (62.5mg / LSB)
void ADXL345__setActivityThreshold(float threshold) {
	uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
	ADXL345__writeRegister8(ADXL345_REG_THRESH_ACT, scaled);
}

// Get Activity Threshold (65.5mg / LSB)
float ADXL345__getActivityThreshold(void) {
	return ADXL345__readRegister8(ADXL345_REG_THRESH_ACT) * 0.0625f;
}

// Set Inactivity Threshold (65.5mg / LSB)
void ADXL345__setInactivityThreshold(float threshold) {
	uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
	ADXL345__writeRegister8(ADXL345_REG_THRESH_INACT, scaled);
}

// Get Incactivity Threshold (65.5mg / LSB)
float ADXL345__getInactivityThreshold(void) {
	return ADXL345__readRegister8(ADXL345_REG_THRESH_INACT) * 0.0625f;
}

// Set Inactivity Time (s / LSB)
void ADXL345__setTimeInactivity(uint8_t time) {
	ADXL345__writeRegister8(ADXL345_REG_TIME_INACT, time);
}

// Get Inactivity Time (s / LSB)
uint8_t ADXL345__getTimeInactivity(void) {
	return ADXL345__readRegister8(ADXL345_REG_TIME_INACT);
}

// Set Free Fall Threshold (65.5mg / LSB)
void ADXL345__setFreeFallThreshold(float threshold) {
	uint8_t scaled = constrain(threshold / 0.0625f, 0, 255);
	ADXL345__writeRegister8(ADXL345_REG_THRESH_FF, scaled);
}

// Get Free Fall Threshold (65.5mg / LSB)
float ADXL345__getFreeFallThreshold(void) {
	return ADXL345__readRegister8(ADXL345_REG_THRESH_FF) * 0.0625f;
}

// Set Free Fall Duratiom (5ms / LSB)
void ADXL345__setFreeFallDuration(float duration) {
	uint8_t scaled = constrain(duration / 0.005f, 0, 255);
	ADXL345__writeRegister8(ADXL345_REG_TIME_FF, scaled);
}

// Get Free Fall Duratiom
float ADXL345__getFreeFallDuration() {
	return ADXL345__readRegister8(ADXL345_REG_TIME_FF) * 0.005f;
}

void ADXL345__setActivityX(bool state) {
	ADXL345__writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 6, state);
}

bool ADXL345__getActivityX(void) {
	return ADXL345__readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 6);
}

void ADXL345__setActivityY(bool state) {
	ADXL345__writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 5, state);
}

bool ADXL345__getActivityY(void) {
	return ADXL345__readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 5);
}

void ADXL345__setActivityZ(bool state) {
	ADXL345__writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 4, state);
}

bool ADXL345__getActivityZ(void) {
	return ADXL345__readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 4);
}

void ADXL345__setActivityXYZ(bool state) {
	uint8_t value;

	value = ADXL345__readRegister8(ADXL345_REG_ACT_INACT_CTL);

	if (state) {
		value |= 0b00111000;
	} else {
		value &= 0b11000111;
	}

	ADXL345__writeRegister8(ADXL345_REG_ACT_INACT_CTL, value);
}

void ADXL345__setInactivityX(bool state) {
	ADXL345__writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 2, state);
}

bool ADXL345__getInactivityX(void) {
	return ADXL345__readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 2);
}

void ADXL345__setInactivityY(bool state) {
	ADXL345__writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 1, state);
}

bool ADXL345__getInactivityY(void) {
	return ADXL345__readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 1);
}

void ADXL345__setInactivityZ(bool state) {
	ADXL345__writeRegisterBit(ADXL345_REG_ACT_INACT_CTL, 0, state);
}

bool ADXL345__getInactivityZ(void) {
	return ADXL345__readRegisterBit(ADXL345_REG_ACT_INACT_CTL, 0);
}

void ADXL345__setInactivityXYZ(bool state) {
	uint8_t value;

	value = ADXL345__readRegister8(ADXL345_REG_ACT_INACT_CTL);

	if (state) {
		value |= 0b00000111;
	} else {
		value &= 0b11111000;
	}

	ADXL345__writeRegister8(ADXL345_REG_ACT_INACT_CTL, value);
}

void ADXL345__setTapDetectionX(bool state) {
	ADXL345__writeRegisterBit(ADXL345_REG_TAP_AXES, 2, state);
}

bool ADXL345__getTapDetectionX(void) {
	return ADXL345__readRegisterBit(ADXL345_REG_TAP_AXES, 2);
}

void ADXL345__setTapDetectionY(bool state) {
	ADXL345__writeRegisterBit(ADXL345_REG_TAP_AXES, 1, state);
}

bool ADXL345__getTapDetectionY(void) {
	return ADXL345__readRegisterBit(ADXL345_REG_TAP_AXES, 1);
}

void ADXL345__setTapDetectionZ(bool state) {
	ADXL345__writeRegisterBit(ADXL345_REG_TAP_AXES, 0, state);
}

bool ADXL345__getTapDetectionZ(void) {
	return ADXL345__readRegisterBit(ADXL345_REG_TAP_AXES, 0);
}

void ADXL345__setTapDetectionXYZ(bool state) {
	uint8_t value;

	value = ADXL345__readRegister8(ADXL345_REG_TAP_AXES);

	if (state) {
		value |= 0b00000111;
	} else {
		value &= 0b11111000;
	}

	ADXL345__writeRegister8(ADXL345_REG_TAP_AXES, value);
}

void ADXL345__useInterrupt(adxl345_int_t interrupt) {
	if (interrupt == 0) {
		ADXL345__writeRegister8(ADXL345_REG_INT_MAP, 0x00);
	} else {
		ADXL345__writeRegister8(ADXL345_REG_INT_MAP, 0xFF);
	}

	ADXL345__writeRegister8(ADXL345_REG_INT_ENABLE, 0xFF);
}

struct Activites ADXL345__readActivites(void) {
	uint8_t data = ADXL345__readRegister8(ADXL345_REG_INT_SOURCE);

	a.isOverrun 	= ((data >> ADXL345_OVERRUN) & 1);
	a.isWatermark 	= ((data >> ADXL345_WATERMARK) & 1);
	a.isFreeFall 	= ((data >> ADXL345_FREE_FALL) & 1);
	a.isInactivity 	= ((data >> ADXL345_INACTIVITY) & 1);
	a.isActivity 	= ((data >> ADXL345_ACTIVITY) & 1);
	a.isDoubleTap 	= ((data >> ADXL345_DOUBLE_TAP) & 1);
	a.isTap 		= ((data >> ADXL345_SINGLE_TAP) & 1);
	a.isDataReady 	= ((data >> ADXL345_DATA_READY) & 1);

	data = ADXL345__readRegister8(ADXL345_REG_ACT_TAP_STATUS);

	a.isActivityOnX = ((data >> 6) & 1);
	a.isActivityOnY = ((data >> 5) & 1);
	a.isActivityOnZ = ((data >> 4) & 1);
	a.isTapOnX 		= ((data >> 2) & 1);
	a.isTapOnY 		= ((data >> 1) & 1);
	a.isTapOnZ 		= ((data >> 0) & 1);

	return a;
}

