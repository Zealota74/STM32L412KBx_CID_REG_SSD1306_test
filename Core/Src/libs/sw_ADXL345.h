/*
 * sw_ADXL345.h
 *
 *  Created on: 2 sie 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_MK_GESTURE_PAJ7620_SW_ADXL345_H_
#define SRC_LIBS_MK_GESTURE_PAJ7620_SW_ADXL345_H_

#define ADXL345_ADDRESS              (0x53 << 1)
#define ADXL345_REG_DEVID            (0x00)
#define ADXL345_REG_THRESH_TAP       (0x1D) // 1
#define ADXL345_REG_OFSX             (0x1E)
#define ADXL345_REG_OFSY             (0x1F)
#define ADXL345_REG_OFSZ             (0x20)
#define ADXL345_REG_DUR              (0x21) // 2
#define ADXL345_REG_LATENT           (0x22) // 3
#define ADXL345_REG_WINDOW           (0x23) // 4
#define ADXL345_REG_THRESH_ACT       (0x24) // 5
#define ADXL345_REG_THRESH_INACT     (0x25) // 6
#define ADXL345_REG_TIME_INACT       (0x26) // 7
#define ADXL345_REG_ACT_INACT_CTL    (0x27)
#define ADXL345_REG_THRESH_FF        (0x28) // 8
#define ADXL345_REG_TIME_FF          (0x29) // 9
#define ADXL345_REG_TAP_AXES         (0x2A)
#define ADXL345_REG_ACT_TAP_STATUS   (0x2B)
#define ADXL345_REG_BW_RATE          (0x2C)
#define ADXL345_REG_POWER_CTL        (0x2D)
#define ADXL345_REG_INT_ENABLE       (0x2E)
#define ADXL345_REG_INT_MAP          (0x2F)
#define ADXL345_REG_INT_SOURCE       (0x30) // A
#define ADXL345_REG_DATA_FORMAT      (0x31)
#define ADXL345_REG_DATAX0           (0x32)
#define ADXL345_REG_DATAX1           (0x33)
#define ADXL345_REG_DATAY0           (0x34)
#define ADXL345_REG_DATAY1           (0x35)
#define ADXL345_REG_DATAZ0           (0x36)
#define ADXL345_REG_DATAZ1           (0x37)
#define ADXL345_REG_FIFO_CTL         (0x38)
#define ADXL345_REG_FIFO_STATUS      (0x39)

#define ADXL345_GRAVITY_SUN          273.95f
#define ADXL345_GRAVITY_EARTH        9.80665f
#define ADXL345_GRAVITY_MOON         1.622f
#define ADXL345_GRAVITY_MARS         3.69f
#define ADXL345_GRAVITY_NONE         1.00f

typedef enum {
	ADXL345_DATARATE_3200HZ = 0b1111,
	ADXL345_DATARATE_1600HZ = 0b1110,
	ADXL345_DATARATE_800HZ = 0b1101,
	ADXL345_DATARATE_400HZ = 0b1100,
	ADXL345_DATARATE_200HZ = 0b1011,
	ADXL345_DATARATE_100HZ = 0b1010,
	ADXL345_DATARATE_50HZ = 0b1001,
	ADXL345_DATARATE_25HZ = 0b1000,
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

#ifndef VECTOR_STRUCT_H
#define VECTOR_STRUCT_H
struct Vector {
	float XAxis;
	float YAxis;
	float ZAxis;
};
#endif

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

extern bool ADXL345__ADXL345__clearSettingsbegin(void);
extern void ADXL345__clearSettings(void);

struct Vector ADXL345__clearSettingsreadRaw(void);
//struct Vector readNormalize(float gravityFactor = ADXL345_GRAVITY_EARTH);
struct Vector ADXL345__clearSettingsreadNormalize(float gravityFactor);
struct Vector ADXL345__clearSettingsreadScaled(void);

struct Activites ADXL345__clearSettingsreadActivites(void);

//struct Vector lowPassFilter(struct Vector vector, float alpha = 0.5);
struct Vector ADXL345__clearSettingslowPassFilter(struct Vector vector,
		float alpha);

extern void ADXL345__setRange(adxl345_range_t range);
extern adxl345_range_t ADXL345__clearSettingsgetRange(void);

extern void ADXL345__setDataRate(adxl345_dataRate_t dataRate);
extern adxl345_dataRate_t ADXL345__clearSettingsgetDataRate(void);

extern void ADXL345__setTapThreshold(float threshold);
extern float ADXL345__getTapThreshold(void);

extern void ADXL345__setTapDuration(float duration);
extern float ADXL345__getTapDuration(void);

extern void ADXL345__setDoubleTapLatency(float latency);
extern float ADXL345__getDoubleTapLatency(void);

extern void ADXL345__setDoubleTapWindow(float window);
extern float ADXL345__getDoubleTapWindow(void);

extern void ADXL345__setActivityThreshold(float threshold);
extern float ADXL345__getActivityThreshold(void);

extern void ADXL345__setInactivityThreshold(float threshold);
extern float ADXL345__getInactivityThreshold(void);

extern void ADXL345__setTimeInactivity(uint8_t time);
extern uint8_t ADXL345__getTimeInactivity(void);

extern void ADXL345__setFreeFallThreshold(float threshold);
extern float ADXL345__getFreeFallThreshold(void);

extern void ADXL345__setFreeFallDuration(float duration);
extern float ADXL345__getFreeFallDuration();

extern void ADXL345__setActivityX(bool state);
extern bool ADXL345__ADXL345__clearSettingsgetActivityX(void);
extern void ADXL345__setActivityY(bool state);
extern bool ADXL345__ADXL345__clearSettingsgetActivityY(void);
extern void ADXL345__setActivityZ(bool state);
extern bool ADXL345__ADXL345__clearSettingsgetActivityZ(void);
extern void ADXL345__setActivityXYZ(bool state);

extern void ADXL345__setInactivityX(bool state);
extern bool ADXL345__getInactivityX(void);
extern void ADXL345__setInactivityY(bool state);
extern bool ADXL345__getInactivityY(void);
extern void ADXL345__setInactivityZ(bool state);
extern bool ADXL345__getInactivityZ(void);
extern void ADXL345__setInactivityXYZ(bool state);

extern void ADXL345__setTapDetectionX(bool state);
extern bool ADXL345__getTapDetectionX(void);
extern void ADXL345__setTapDetectionY(bool state);
extern bool ADXL345__getTapDetectionY(void);
extern void ADXL345__setTapDetectionZ(bool state);
extern bool ADXL345__getTapDetectionZ(void);
extern void ADXL345__setTapDetectionXYZ(bool state);

extern void ADXL345__useInterrupt(adxl345_int_t interrupt);

#endif /* SRC_LIBS_MK_GESTURE_PAJ7620_SW_ADXL345_H_ */
