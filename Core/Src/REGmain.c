#include "sw_mcu_conf.h"

#include "libs/SW_STM_PERIPHS/sw_rcc.h"

#include "libs/SW_BOARD/sw_gpio.h"
#include "libs/SW_BOARD/sw_led_blink_debug.h"
#include "libs/SW_TIMERS/sw_soft_timers.h"
#include "libs/SW_STM_PERIPHS/sw_i2c_base_v2.h"

#include "libs/SW_GRAPHICS/SW_SSD1306/framebuffer.h"
#include "libs/SW_GRAPHICS/displays.h"
#include "libs/SW_GRAPHICS/text.h"
#include "libs/SW_GRAPHICS/SW_SSD1306/sw_ssd1306_v1.h"

#include "libs/SW_SENSORS/sw_mpu6050.h"
#include "libs/SW_SENSORS/sw_ADXL345.h"
#include "libs/SW_SENSORS/VL53L0X.h"


#include "libs/SW_SENSORS/sw_vcnl4010.h"
#include "libs/SW_INPUT/keyboard.h"

void keyboard(void);
void SystemClock_Config(void);

void pomiar( T_RESULTS *results );
uint16_t proximityAverage;
uint16_t ambilightAverage;

void mpu6050_test_loop(void);
bool mpu6050_test_init(void);

T_STRING TextX;
T_STRING TextY;
T_STRING TextZ;
T_STRING Temp;

FONT_INFO CurrentFont;

uint8_t zmienna1, zmienna2, zmienna3;
void key_proc1(void);

int main(void) {
	SystemClock_Config();
	RCC_gpio_init();
	sw_softTimers_init( 1, MICRO_SEC );

	sw_i2c_simple_init();
	sw_led_debug_init();
	sw_keyboard_init();

	SysTick_Config( SystemCoreClock / 1000 );	// Systick on 1 ms

	delay_ms(100);

	ADXL345__init();

	sw_ssd1306_init();
	delay_ms(100);

//	VL53L0X__setup();
	delay_ms(100);

	delay_ms(100);
//	mpu6050_test_init();
//	MPU6050__setThreshold(3);
//	MPU6050__calibrateGyro(50);

//	I2CSTATUS status = sw_i2c_IsDeviceReady( ADDRESS_DEFAULT << 1, 3, 3 );
//	sw_i2c_slave_test( ADDRESS_DEFAULT << 1 );

	softTimer3 = 200;
	register_measure_callback( pomiar );

//	paj7620_init( fps_120 );
	delay_ms(100);

//    register_gesture_callback( my_gesture, NULL );


	softTimer3 = 500;
	while(1) {

//		SW_VCNL4010_MEASURE_EVENT();
//		PAJ7620_EVENT();

		if (softTimer2 == 0) {
			softTimer2 = 200;
//			sw_led_xor();

//			mpu6050_test_loop();
//			VL53L0X__loop();
		}
		if ( !softTimer3 ) {
			softTimer3 = 100;
		}
	}
}


void key1_decoded(void) {
	static int8_t counter1, counter2, counter3;
	if ( keyboard_ptr()->keyEvent == SHORT_PRESS ) {
//		sw_led_on();
//		sw_led_start_blinking( 2, 100 );
		TEXT_display_float( 0, 0,  ++counter1,  &TextX );
	} else
	if ( keyboard_ptr()->keyEvent == MEDIUM_PRESS ) {
		TEXT_display_float( 0, 16,  ++counter2, &TextY );
	} else
	if ( keyboard_ptr()->keyEvent == DOUBLE_PRESS ) {
		TEXT_display_float( 0, 32,  ++counter3, &TextZ );
	} else
	if ( keyboard_ptr()->keyEvent == VLONG_PRESS ) {
		counter3 = 0; counter2 = 0; counter1 = 0;
		TEXT_display_float( 0,  0, counter1,  &TextX );
		TEXT_display_float( 0, 16, counter2,  &TextY );
		TEXT_display_float( 0, 32, counter3,  &TextZ );
		sw_led_start_blinking( 3, 100 );
	} else
	if ( keyboard_ptr()->keyEvent == KEY_REPEAT ) {
		TEXT_display_float( 0, 0,  ++counter1,  &TextX );
	}
	TEXT_display_string( 0, 48, (CHAR *)"Simon śćźń", &Temp );
	sw_ssd1306_display();
}
void key2_decoded(void) { sw_led_xor(); }


#define MEDIAL	20
static float32_t srednia1( float32_t wartosc ) {
	static float32_t bufor1[MEDIAL] 		= {0};
	static uint16_t kolejny_pomiar = 0;
	bufor1[ kolejny_pomiar++ ] = wartosc;
	if (kolejny_pomiar == MEDIAL) {
		kolejny_pomiar = 0;
	}

	float32_t wynik = 0;
	for ( uint8_t i=0; i<MEDIAL; i++ ) {
		wynik = wynik + bufor1[i];
	}
	wynik = (float32_t)wynik / MEDIAL;
	return wynik;
}
bool mpu6050_test_init(void) {
//	if( false == MPU6050__init( MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G ) ) {
//		return false;
//	}
//	MPU6050__calibrateGyro( 5 );	// Kalibracja żyroskopu
//	MPU6050__setThreshold( 3 );		// Ustawienie czułości

	sw_mpu6050_test_init();

	return true;
}
void mpu6050_test_loop(void) {
//	struct Vector rawGyro;
//	struct Vector normGyro;


//	rawGyro		= MPU6050__readRawGyro();
//	rawGyro		= MPU6050__readRawAccel();
//	normGyro	= MPU6050__readNormalizeAccel();
//	normGyro	= MPU6050__readNormalizeGyro();
//	normGyro.XAxis = srednia1( normGyro.XAxis );
//	TEXT_display_float( 0, 0,  normGyro.XAxis, &TextX );
//	TEXT_display_float( 0, 16, normGyro.YAxis, &TextY );
//	TEXT_display_float( 0, 32, normGyro.ZAxis, &TextZ );

// Calculate Pitch, Roll and Yaw
//	static unsigned long 	timer 	 = 0;
//	float 					timeStep = 0.01;

//	timer = millis();
//	float pitch = 0;
//	float roll  = 0;
//	float yaw   = 0;
//	pitch 	= pitch + normGyro.YAxis * timeStep;
//	roll 	= roll  + normGyro.XAxis * timeStep;
//	yaw 	= yaw   + normGyro.ZAxis * timeStep;
//	TEXT_display_float( 0, 0,  pitch, &TextX );
//	TEXT_display_float( 0, 16, roll,  &TextY );
//	TEXT_display_float( 0, 32, yaw,	  &TextZ );
//	delay_ms( (timeStep*1000) - (millis() - timer));

//	struct Vector normAccel = MPU6050__readNormalizeAccel();
//
//	int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0) / M_PI;
//	int roll  =  (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0) / M_PI;
//	TEXT_display_number( 0, 0,  pitch, &TextX );
//	TEXT_display_number( 0, 16, roll, &TextY );
//	float temp = MPU6050__readTemperature();
//	TEXT_display_float( 0, 48, temp, &Temp );

//	int16_t acc_x, acc_y, acc_z, gyr_x, gyr_y, gyr_z;
	float ax, ay, az, gx, gy, gz, temperature, roll, pitch, yaw;
	MPU6050_GetAccelerometerScaled( &ax, &ay, &az );
	MPU6050_GetGyroscopeScaled	  ( &gx, &gy, &gz );
	temperature = MPU6050_GetTemperatureCelsius();

	MPU6050_GetRollPitch( &roll, &pitch, &yaw );
//	T_STRING TextX;
	temperature = srednia1(roll);
	TEXT_display_float( 0, 0,  roll,  	&TextX );
	TEXT_display_float( 0, 16, pitch, 	&TextY );
	TEXT_display_float( 0, 32, yaw,		&TextZ );

//	TEXT_display_float( 0, 48, temperature, &Temp );

}

// ------------ Definicje funkcji --------------

static uint16_t srednia2( uint16_t wartosc ) {
	static uint16_t bufor2[5] = {0,0,0,0,0};
	static uint8_t kolejny_pomiar = 0;
	bufor2 [ kolejny_pomiar++ ] = wartosc;
	if (kolejny_pomiar == 5) {
		kolejny_pomiar = 0;
	}

	uint32_t wynik = 0;
	for ( uint8_t i=0; i<5; i++ ) {
		wynik = wynik + bufor2[i];
	}
	wynik = (uint16_t)wynik / 5;
	return wynik;
}


void pomiar( T_RESULTS *pomiar1 ) {
	uint16_t result;

	// Składamy wynik 16 bitowy z 8 bitowych
	result = ( (uint16_t)pomiar1->proximityHigh<<8 ) | pomiar1->proximityLow;
	proximityAverage = srednia1(result ),
//	glcd_int( 0, 0, result, 1 );

	result = ( (uint16_t)pomiar1->ambientHigh<<8 )   | pomiar1->ambientLow;
	ambilightAverage = srednia2(result);
//	glcd_int( 0, 36, result, 1 );

	gpio_pin_XOR( DEBUG_PORT0, DEBUG_PIN0 );
}

