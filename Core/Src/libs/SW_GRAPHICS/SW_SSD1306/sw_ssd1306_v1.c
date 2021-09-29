/*
 * rgb_matrix_64x32.c
 *
 *  Created on: 11.04.2017
 *      Author: simon
 *
 * 	All depends on compiler optimization
 * 	"-O0", "-O1", "-Os", "-Og" works fine in all FRAMEBUFFER_MAXBITDEPTH values
 * 	"-Os", "-O2", "-O3" don't work OE signal
 * 	With "-O2" works only for MAXBITDEPTH > 4 ???
 * 	Maybe something wrong with DMA??
 * 	8 and 1 bit depth don't work at all - it have to be correct
 * 	Maximum dataClock = 3MHz, speeder values gives artifact
 * 	We need optimization in DMA routines, I think...
 */


#include "sw_mcu_conf.h"

#include "../../SW_BOARD/sw_gpio.h"
#include "../../SW_BOARD/sw_led_blink_debug.h"
#include "../../SW_STM_PERIPHS/sw_dma.h"
//#include "../../SW_STM_PERIPHS/sw_tim_v2.h"
#include "../../SW_TIMERS/sw_soft_timers.h"
#include "../../SW_STM_PERIPHS/sw_i2c_base_v2.h"

#include "framebuffer.h"
#include "../colors.h"
#include "sw_ssd1306_v1.h"

static uint8_t bright = 5;

volatile static T_DISPLAY * ActualDMABuffer;
volatile uint8_t scrollTrigs = 0;

volatile uint16_t framerate = 240;

static uint16_t _width		= SSD1306_WIDTH;
static uint16_t _height 	= SSD1306_HEIGHT;

// Wysyłanie danej przez SPI
//**********************************************
#ifdef SSD1306_SPI
void WriteSpi( uint8_t data ) {
	uint8_t mask = 0x80;
	for ( uint16_t i=0; i<8; i++ ) {
		SCK_LO;
			(data&mask) ? MOSI_HI : MOSI_LO;
		SCK_HI;
		mask >>= 1;
	}
}
#endif
//***********************************************
// Zapisanie kodu komendy do sterownika SSD1306
//***********************************************
static void WriteCmd( uint8_t cmd ) {
#if TRANSMISSION_MODE==SOFT_SPI
	#if USE_CS==1
	CS_HI;
	#endif
	DC_LO;
	#if USE_CS==1
	CS_LO;
	#endif
//	sw_software_SPI_write( cmd );
	sw_transmision_write( cmd );
	#if USE_CS==1
	CS_HI;
	#endif
#elif (TRANSMISSION_MODE==SOFT_I2C) || (TRANSMISSION_MODE==HARD_I2C)
	sw_i2c_write_reg8( I2C_ADDRESS, I2C_CTRL_DC_0, cmd );
#endif
	delay_us(20);
}
//***********************************************
// Zapisanie danej do pamięci obrazu sterownika SSD1306
//***********************************************
static void WriteData( uint8_t regAddr, uint8_t * buffer, uint16_t cnt ) {
#ifdef SSD1306_SPI
	CS_HI;
	DC_HI;      							// DC=1 przesłanie danej do pamięci RAM
	CS_LO;
	for (uint16_t i=0; i < cnt; i++ ) {
		WriteSpi( buffer[i] ); 				// zapis danej
	}
	CS_HI;
#endif
#ifdef SSD1306_I2C
	sw_i2c_write_bulk( SSD1306_ADDR, SSD1306_SETSTARTLINE, cnt, buffer );
#endif
}

//**********************************************
// Zeruj bufor wyświetlacza
// i zapisz jego zwartość do RAM obrazu
//**********************************************
void sw_display_cls( uint8_t *buffer, uint8_t pattern ) {
	for ( uint16_t i = 0; i < FRAMEBUFFER_ALL; i++ ) {
		buffer[i] = pattern;
	}
}
//***********************************************

//************ Wyślij bufor RAM do sterownika ************
void sw_ssd1306_display( void ) {
	sw_i2c_set_7bitAddr( 0x78 );

	WriteCmd( 0x21  );				// Command_Column_Address_Set
	WriteCmd( 0x00  );				// Start
	WriteCmd( SCREEN_WIDTH  - 1  );	// End

	WriteCmd( 0x22  );				// Command_Page_Address_Set
	WriteCmd( 0x00  );				// Start
	WriteCmd( SCREEN_HEIGHT - 1 );	// End

	WriteCmd( SSD1306_SETLOWCOLUMN  );
	WriteCmd( SSD1306_SETHIGHCOLUMN );
#if SCREEN_HEIGHT == 64
	WriteCmd( 0xB0 );
#endif
#if SCREEN_HEIGHT == 32
	WriteCmd( 0xB4 );
#endif
//	delay_ms(100);
   	WriteData( SSD1306_SETSTARTLINE, (uint8_t *)ActualDMABuffer, FRAMEBUFFER_ALL );
	delay_us(20);
}
/***************************************************************/

/*********************************** Timer16 init **************************************/
static void timer16_init( uint16_t timeFPS ) {
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
	TIM16->PSC = (uint32_t)SystemCoreClock / 0xFFFF / timeFPS - 1;
	TIM16->ARR = 0xFFFF - 1;

	TIM16->DIER	= TIM_DIER_UIE;		// Update Event Interrupt

	TIM16->CR1 |= TIM_CR1_CEN;

	NVIC_EnableIRQ( TIM1_UP_TIM16_IRQn );
}
INTERRUPT CCMRAM void TIM1_UP_TIM16_IRQHandler(void) {	// Timer2 Interrupt Handler
	if ( TIM16->SR & TIM_SR_UIF ) {
		TIM16->SR &= ~TIM_SR_UIF;						// Update interrupt Flag

		if (refreshScreen == 0) return;
		ActualDMABuffer = TextBuffer;
		sw_ssd1306_display();
		refreshScreen = 0;
		gpio_pin_XOR( DEBUG_PORT1, DEBUG_PIN1 );
	}
}
/****************************************************************************************/

//************* INICJALIZACJA sterownika SSD1306
void sw_ssd1306_init(void) {
//	ActualDMABuffer = SpectrumBuffer;
//	ActualDMABuffer = TextBufferDisplay;
	ActualDMABuffer = TextBuffer;
#ifdef SSD1306_SPI
	gpio_pin_cfg( SPI_NSS_PORT, 	SPI_NSS_PIN, gpio_mode_output_PP_HS );
	gpio_pin_cfg( SPI_SCK_PORT, 	SPI_SCK_PIN, gpio_mode_output_PP_HS );
	gpio_pin_cfg( SPI_DC_PORT,		SPI_DC_PIN,  gpio_mode_output_PP_HS );
	gpio_pin_cfg( SPI_MOSI_PORT,	SPI_MOSI_PIN,gpio_mode_output_PP_HS );
	gpio_pin_cfg( RES_PORT,			RES_PIN,	 gpio_mode_output_PP_HS );

//	delay_ms(100);
	RES_LO;
	RES_HI;
#endif
	sw_i2c_set_7bitAddr( 0x78 );

	WriteCmd( SSD1306_DISPLAYOFF );
	WriteCmd( SSD1306_SETDISPLAYCLOCKDIV );
	WriteCmd( OLED_CONTRAST );

	WriteCmd( SSD1306_SETDISPLAYOFFSET );
	WriteCmd( 0x0 );
	WriteCmd( SSD1306_SETSTARTLINE | 0x0 );
	WriteCmd( SSD1306_CHARGEPUMP );

//    if (vcc == SSD1306_EXTERNALVCC ) WriteCmd( 0x10 );
//    else  WriteCmd( 0x14 );	// SSD1306_SWITCHCAPVCC

	WriteCmd( 0x14 );			// SSD1306_SWITCHCAPVCC

	WriteCmd( SSD1306_MEMORYMODE );
	WriteCmd( 0x00 );
	WriteCmd( SSD1306_SEGREMAP | 0x1 );
	WriteCmd( SSD1306_COMSCANDEC );

	WriteCmd( SSD1306_SETCONTRAST );
	WriteCmd( REFRESH_MAX );

//	if (vcc == SSD1306_EXTERNALVCC ) WriteCmd( 0x9F );
//	else WriteCmd( 0xCF );

	WriteCmd( 0xCF );					// SSD1306_SWITCHCAPVCC
	WriteCmd( SSD1306_SETPRECHARGE );

// ssd1306 - 128 x 32
#ifdef GLCD_RES_128_32
	WriteCmd( SSD1306_SETMULTIPLEX );
	WriteCmd( 0x1F );

	WriteCmd( SSD1306_SETCOMPINS );
	WriteCmd( 0x02 );
#endif
// ssd1306 - 128 x 64
#ifdef GLCD_RES_128_64
	WriteCmd( SSD1306_SETMULTIPLEX );
	WriteCmd( 0x3F );

	WriteCmd( SSD1306_SETCOMPINS );
	WriteCmd( 0x12 );
#endif
	WriteCmd( SSD1306_SETVCOMDETECT );
	WriteCmd( 0x40 );

	WriteCmd( SSD1306_DISPLAYALLON_RESUME );
	WriteCmd( SSD1306_NORMALDISPLAY );
	WriteCmd( SSD1306_DISPLAYON );

	sw_display_cls( (uint8_t *)ActualDMABuffer, 0x00 );
	sw_ssd1306_display();

	timer16_init( 10 );
	sw_dma_memset_init(8);
}
/***************************************************************************************/

/*********************************** Setters i getters ********************************/
uint8_t *set_get_bright( uint16_t brightLoc ) {
	return &bright;
}
uint8_t *set_get_bright2( uint8_t cnt ) {
	return &bright;
}
/***************************************************************************************/

/*************************************************************************************************/
// Funkcje oparte na rysowaniu pojedynczego piksela
void graphic_set_pixel_RAM( volatile T_DISPLAY *buffer, int16_t x, int16_t y, const T_COLOR *color ) {
	  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
	    return;
	  if ( *color )
		  buffer[ x + (y/8)*_width ] |=  (1<<(y%8)); // zapal pixel
	  else
		  buffer[ x + (y/8)*_width ] &= ~(1<<(y%8)); // zgaś pixel
}
/***************************************************************************************/
