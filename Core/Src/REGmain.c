#include "sw_mcu_conf.h"

#include "libs/sw_rcc.h"

#include "libs/SW_BOARD/sw_gpio.h"
#include "libs/SW_BOARD/sw_led_blink_debug.h"
#include "libs/SW_TIMERS/sw_soft_timers.h"
#include "libs/sw_i2c_simple_v2.h"

#include "libs/SW_GRAPHICS/SW_SSD1306/framebuffer.h"
#include "libs/SW_GRAPHICS/displays.h"
#include "libs/SW_GRAPHICS/text.h"
#include "libs/SW_GRAPHICS/SW_SSD1306/sw_ssd1306_v1.h"

#include "libs/MK_GESTURE_PAJ7620/mk_paj7620.h"
#include "libs/sw_mpu6050.h"


#include "libs/sw_vcnl4010.h"


void SystemClock_Config(void);

void pomiar( T_RESULTS *results );
uint16_t proximityAverage;
uint16_t ambilightAverage;
void my_gesture( TGSNR last_gs, TGSNR second_gs, TGSNR first_gs );

FONT_INFO CurrentFont;

int main(void) {
	SystemClock_Config();
	RCC_gpio_init();
	sw_led_debug_init();
	sw_softTimers_init( 1, MICRO_SEC );

	delay_ms(100);
	sw_i2c_simple_init();
	sw_ssd1306_init();

	MPU6050__init( MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G );
	MPU6050__setThreshold( 3 );		// Ustawienie czułości
	MPU6050__calibrateGyro( 5 );	// Kalibracja żyroskopu

	delay_ms(10);

	if ( vcnl4010_init() ) {
//		glcd_puts( 0, 0, "VCNL4010 initialized", 1 );

	} else {
//		glcd_puts( 0, 0, "ERROR", 1 );
	}
	delay_ms(1000);

	softTimer3 = 200;
	register_measure_callback( pomiar );

	paj7620_init( fps_120 );
    register_gesture_callback( my_gesture, NULL );

    T_STRING TextX;
    T_STRING TextY;
    T_STRING TextZ;

	softTimer3 = 500;
	struct Vector rawGyro;
	struct Vector normGyro;
	while(1) {
//		SW_VCNL4010_MEASURE_EVENT();
//		PAJ7620_EVENT();

		if (softTimer2 == 0) {
			softTimer2 = 500;
			sw_led_xor();
//			rawGyro		= MPU6050__readRawGyro();
//			normGyro	= MPU6050__readNormalizeGyro();
			normGyro	= MPU6050__readNormalizeAccel();
			TEXT_display_float( 0, 0,  normGyro.XAxis, &TextX );
			TEXT_display_float( 0, 16, normGyro.YAxis, &TextY );
			TEXT_display_float( 0, 32, normGyro.ZAxis, &TextZ );

		}

		if ( !softTimer3 ) {
			sw_ssd1306_display();
			softTimer3 = 300;
		}
	}
}



// ------------ Definicje funkcji --------------
static uint16_t srednia1( uint16_t wartosc ) {
	static uint16_t bufor1[5] = {0,0,0,0,0};
	static uint8_t kolejny_pomiar = 0;
	bufor1[ kolejny_pomiar++ ] = wartosc;
	if (kolejny_pomiar == 5) {
		kolejny_pomiar = 0;
	}

	uint32_t wynik = 0;
	for ( uint8_t i=0; i<5; i++ ) {
		wynik = wynik + bufor1[i];
	}
	wynik = (uint16_t)wynik / 5;
	return wynik;
}

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

void my_gesture( TGSNR last_gs, TGSNR second_gs, TGSNR first_gs ) {
//	graphic_fill_screen_RAM( BLACK );
    																	// numery gestów
//    if( last_gs 	 == gs_right )      graphic_puts_RAM( 0, 0, L"RIGHT", 1, WHITE, BLACK, CurrentFont );		// 1
//    else if( last_gs == gs_left )       graphic_puts_RAM( 0, 0, L"LEFT", 1, WHITE, BLACK, CurrentFont );		// 2
//    else if( last_gs == gs_up )         graphic_puts_RAM( 0, 0, L"UP", 1, WHITE, BLACK, CurrentFont );		// 3
//    else if( last_gs == gs_down )       graphic_puts_RAM( 0, 0, L"DOWN", 1, WHITE, BLACK, CurrentFont );		// 4
//    else if( last_gs == gs_forward )    graphic_puts_RAM( 0, 0, L"FORWARD", 1, WHITE, BLACK, CurrentFont );	// 5
//    else if( last_gs == gs_backward )   graphic_puts_RAM( 0, 0, L"BACKWARD", 1, WHITE, BLACK, CurrentFont );	// 6
//    else if( last_gs == gs_cw )         graphic_puts_RAM( 0, 0, L"CW", 1, WHITE, BLACK, CurrentFont );		// 7
//    else if( last_gs == gs_ccw )        graphic_puts_RAM( 0, 0, L"CCW", 1, WHITE, BLACK, CurrentFont );		// 8
//    else if( last_gs == gs_wave )       graphic_puts_RAM( 0, 0, L"WAVE", 1, WHITE, BLACK, CurrentFont );		// 9

//	sw_ssd1306_ram_to_display(0);
}
