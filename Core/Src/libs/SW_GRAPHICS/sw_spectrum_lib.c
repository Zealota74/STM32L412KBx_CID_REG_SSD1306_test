/*
 * spectrum_lib.c
 *
 *  Created on: 25 lut 2016
 *      Author: simon
 */
#include "arm_math.h"

#include "sw_mcu_conf.h"
#include "../SW_BOARD/gpio.h"
#include "../SW_BOARD/sw_led_blink_debug.h"

#include "../SW_TIMERS/sw_soft_timers.h"
#include "../SW_STM_PERIPHS/sw_tim_v2.h"
#include "../SW_STM_PERIPHS/sw_adc_v2.h"

#include "../SW_DSP/sw_dsp.h"

#include "SW_SSD1306/framebuffer.h"
#include "displays.h"
#include "SW_SSD1306/sw_ssd1306_v1.h"

#include "menu.h"
#include "sw_spectrum_lib.h"

volatile uint8_t  calculate = 1;

static T_BAR SpectrumBarsMAIN[ BARS_CNT_MAX ];	// Tablica struktur ustalających parametry dynamiczne słupków
static T_BAR SpectrumBarsAVG;
//const	T_BAR 		*spectrumPTR = SpectrumBarsMAIN;

static	T_SPECTRUM	SpectrumSetsMAIN;			// Struktura definiująca parametry całego spektrum
static	T_SPECTRUM	SpectrumSetsAVG;
T_COLORS	SpectrumColors;

static uint8_t 	 barsCNT 			 = BARS_CNT_MAX;
uint16_t 		*TableIterPTR;
static	 bool 	 calculateEndFftFlag = 0;		// Flaga gotowości na obliczanie transformaty
static	 uint8_t frequency		 	 = 4;		//
volatile bool	 fftCalculate 		 = 0;

static void set_bars_number( uint8_t bars, uint8_t barWidth, uint8_t barGap ) {
	SpectrumSetsMAIN.barsCNT 	= bars;
	SpectrumSetsMAIN.barWidth	= barWidth;
	SpectrumSetsMAIN.peakWidth	= barWidth;

	for ( uint8_t i = 0; i < barsCNT; i++ ) {
		SpectrumBarsMAIN[ i ].xStart = SpectrumSetsAVG.barWidth + 2 + i*( barWidth + barGap );
	}
}

static void set_bars_colors( T_COLORS * CurrentColors, const T_COLORS * PresetColors ) {
	CurrentColors->barMin	= PresetColors->barMin;
	CurrentColors->barMid	= PresetColors->barMid;
	CurrentColors->barMax	= PresetColors->barMax;
	CurrentColors->barPeak	= PresetColors->barPeak;
}
/****************** Definiuje parametry słupków w SpectrumSetsMAIN *******************/
void sw_spectrum_init( uint8_t spectrumMode ) {

	// Rysujemy linię bazową u podstawy
	if ( *set_driver_mode() == DRIVER_MODE_1 )
		graphic_fill_rectangle_RAM( SpectrumBuffer, 0, 31, SCREEN_WIDTH, 1, WhiteColor, &GammaRGB );
	else {
		graphic_fill_circle( SpectrumBuffer,  	 16, 15, 15, YellowColor );
		graphic_fill_circle( TextBufferProgram,  64, 15, 15, CyanColor );
		graphic_fill_circle( TextBufferDisplay, 112, 15, 15, MagentaColor );
	}
	set_bars_colors( &SpectrumColors, &DefaultColors );

	SpectrumSetsMAIN.barType		= barContinuos;
	SpectrumSetsMAIN.barWidth		= 1;
	SpectrumSetsMAIN.peakWidth		= 1;
	SpectrumSetsMAIN.barThick		= 1;
	SpectrumSetsMAIN.peakThick		= 1;
	SpectrumSetsMAIN.barGap			= 0;
	SpectrumSetsMAIN.peakJump		= 0;

	SpectrumSetsMAIN.barsCNT		= barsCNT;
	SpectrumSetsMAIN.barHeightMax	= 31;

	SpectrumSetsAVG.barWidth	= 8;
	SpectrumSetsAVG.peakWidth	= 8;
	SpectrumSetsAVG.barThick	= 1;
	SpectrumSetsAVG.peakThick	= 1;
	SpectrumSetsAVG.barGap		= 1;
	SpectrumSetsAVG.peakJump	= 0;

	SpectrumSetsAVG.barsCNT		= 1;
	SpectrumSetsAVG.barHeightMax= 31;

	SpectrumBarsAVG.yStart 		= Y_START;
	SpectrumBarsAVG.xStart 		= 0;
	SpectrumBarsAVG.lastHeight	= 0;
	SpectrumBarsAVG.height		= 0;
	SpectrumBarsAVG.peakHeight	= 31;

	for ( uint8_t i = 0; i < BARS_CNT_MAX; i++ ) {
		// Efekt podstawowy
		SpectrumBarsMAIN[i].yStart 		= Y_START;
		SpectrumBarsMAIN[i].xStart 		= SpectrumSetsAVG.barWidth + 2 + i*(SpectrumSetsMAIN.barWidth + SpectrumSetsAVG.barGap);
		SpectrumBarsMAIN[i].lastHeight	= 0;
		SpectrumBarsMAIN[i].height		= 0;
		SpectrumBarsMAIN[i].peakHeight	= 31;
	}
	sw_fft_init();
	sw_tim7_init();				// Draw the SpectrumSetsMAIN in RAM
	sw_timer_start( TIM7 );		// Draw the spectrum in RAM

	set_bars_number( barsCNT, 1, 1 );
}
/******************************************************************************/

/******************************************************************************/
void clear_spectrum_RAM( void ) {
	clear_buffer_RAM( SpectrumBuffer );

	for (uint8_t i=0; i < barsCNT; i++ ) {
		SpectrumBarsMAIN[i].lastHeight	= 0;
		SpectrumBarsMAIN[i].height		= 0;
		SpectrumBarsMAIN[i].peakHeight	= 31;
	}
	SpectrumBarsAVG.lastHeight 	= 0;
	SpectrumBarsAVG.height 		= 0;
	SpectrumBarsAVG.peakHeight 	= 31;

	graphic_fill_rectangle_RAM( SpectrumBuffer, 0, 31, SCREEN_WIDTH, 1, WhiteColor, &GammaRGB );
}
/******************************************************************************/

/******************************************************************************/
void sw_visualisation_mode( void ) {
	static uint8_t spectrumMode = 1;
	fftCalculate = 0;
	switch( spectrumMode++%2 ) {
		case SPECTRUM_MODE_BARS:
			*set_driver_mode() = SPECTRUM_MODE_BARS+1;
			clear_buffer_RAM( TextBufferDisplay );
			clear_buffer_RAM( TextBufferProgram );
			clear_spectrum_RAM();
			break;
		case SPECTRUM_MODE_COLOROFON:
			*set_driver_mode() = SPECTRUM_MODE_COLOROFON+1;
			clear_buffer_RAM( TextBufferDisplay );
			clear_buffer_RAM( TextBufferProgram );
			clear_buffer_RAM( SpectrumBuffer );
			graphic_fill_circle( SpectrumBuffer,  	 16, 15, 15, YellowColor );
			graphic_fill_circle( TextBufferProgram,  64, 15, 15, CyanColor );
			graphic_fill_circle( TextBufferDisplay, 112, 15, 15, MagentaColor );
			break;
		default:
			break;
	}
	fftCalculate = 1;
}
/******************************************************************************/

/******************************************************************************/
/**************** Ustawiamy parametry spektrum ********************************/
void set_spectrum_param( uint8_t mode ) {
	static uint8_t lastMode;

	if ( lastMode == mode ) return;		// Wychodzimy jeśli bez zmian

	switch ( mode ) {
		case 3:							// Wartości 0-3 ustawiają parametry spektrum
			barsCNT = 24;
//			TableIterPTR = (uint16_t *)&filtersIter23[0][0];
			set_bars_number( barsCNT, 4, 1 );
			SpectrumSetsMAIN.peakThick	= 1;
			break;
		case 1:
			barsCNT = 16;
//			TableIterPTR = (uint16_t *)&filtersIter16[0][0];
			set_bars_number( barsCNT, 6, 1 );
			SpectrumSetsMAIN.peakThick	= 1;
			break;
		case 2:
			barsCNT = 8;
//			TableIterPTR = (uint16_t *)&filtersIter8[0][0];
			set_bars_number( barsCNT, 13, 1 );
			SpectrumSetsMAIN.peakThick	= 1;
			break;
		case 0:
			barsCNT = 64;
//			TableIterPTR = (uint16_t *)&filtersIter63[0][0];
			set_bars_number( barsCNT, 1, 1 );
			SpectrumSetsMAIN.peakThick	= 1;
			break;
		default:
			frequency = mode;			// Wartości 4-6 ustawiają
			break;						// zakres badanych częstotliwości
	}
	lastMode = mode;
};
/******************************************************************************/

/*********************** Ustawiamy kolory spektrum ****************************/
void set_spectrum_colors( uint8_t colors ) {
	static uint8_t lastColors;
	if ( lastColors == colors ) return;

	switch ( colors ) {
		case defaultColor:
			set_bars_colors( &SpectrumColors, &DefaultColors );
		break;
		case altColor1:
			set_bars_colors( &SpectrumColors, &AltColor1 );
		break;
		case altColor2:
			set_bars_colors( &SpectrumColors, &AltColor2 );
		break;
		case altColor3:
			set_bars_colors( &SpectrumColors, &AltColor3 );
		break;
		case altColor4:
			set_bars_colors( &SpectrumColors, &AltColor4 );
		break;
		case altColor5:
			set_bars_colors( &SpectrumColors, &AltColor5 );
		break;
		case altColor6:
			set_bars_colors( &SpectrumColors, &AltColor6 );
		break;
		case altColor7:
			break;
		case altColor8:
			break;
		default:
		break;
	}
	lastColors = colors;
}
/******************************************************************************/

/******************************************************************************/
static INLINE float32_t dB( float32_t x ) {
  if (x == 0) {
    return (float32_t)1;
  }
  else {
    return (float32_t)( 20*log10f(x) );
  }
}
/******************************************************************************/

/***********************************************************************************/
static INLINE int32_t map	( int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max ) {
	return  (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
static INLINE int32_t db_map( int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max ) {
	if( x < in_min ) return out_min;
	if( x > in_max ) return out_max;

	return map( x, in_min, in_max, out_min, out_max );
}
static INLINE int32_t f32_to_i32_map( float32_t x, float32_t in_min, float32_t in_max, int32_t out_min, int32_t out_max ) {
	x = (float32_t)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	return (int32_t)x;
}

OPTIMISE(0) static inline float32_t select_value_range_128( volatile uint8_t iter ) {
	volatile float32_t temp = adc_div(ADC_CFGR_RES_8BIT);

	temp = (0
		  + bufferMagPTR[4*iter-2]
		  + bufferMagPTR[4*iter+1]
		  + bufferMagPTR[4*iter+2]
		  + bufferMagPTR[4*iter+3]
						 	 	 	 ) / adc_div(ADC_CFGR_RES_8BIT)
		  ;
	temp = (float32_t)temp*temp/16;
	return ( temp );
}
/************************** Oblicza wysokości słupków ******************************/
OPTIMISE(0) static void calculate_bars( volatile uint8_t scale ) {
	if ( calculate == 0 ) return;

	volatile uint32_t	tmp_u32 	= 0U;
	volatile float32_t	tmp_float 	= 0;
	volatile uint32_t	average 	= 0U;

	switch( frequency ) {
		case 4:
			for ( volatile uint8_t i = 2; i < barsCNT - 2; i++ )  {
				tmp_float = select_value_range_128( i );// << scale;
				tmp_u32 = f32_to_i32_map( tmp_float, 0.0, 1.0/(1000*scale), 0, 30 );
//				tmp_float =  dB( tmp_float );
				// Average from 4 neighbor samples
				tmp_u32 = (uint32_t)( SpectrumBarsMAIN[i-2].height + SpectrumBarsMAIN[i-1].height
											+ tmp_u32 +
									  SpectrumBarsMAIN[i+1].height + SpectrumBarsMAIN[i+2].height )/5;

				SpectrumBarsMAIN[i].height = (uint8_t) tmp_u32;

				if ( SpectrumBarsMAIN[i].height > SpectrumSetsMAIN.barHeightMax - SpectrumSetsMAIN.peakThick ) {
					 SpectrumBarsMAIN[i].height = SpectrumSetsMAIN.barHeightMax - SpectrumSetsMAIN.peakThick;
				}
				average += (uint16_t)SpectrumBarsMAIN[i].height;
			}
			break;
		default:
			break;
	}
}
/***********************************************************************************/
#ifdef WAVE_DRAW
/***********************************************************************************/
static void draw_wave( volatile T_DISPLAY * buffer ) {
	uint8_t yPos = 0;
	volatile uint16_t index = 0;
	for ( uint16_t i = 0; i < 1024; i++ )  {
		if ( (adcBuffPTR[ i ] >= 100) && (adcBuffPTR[ i + 1 ] < 105) && (adcBuffPTR[ i + 2 ] < 105)
				&& (i < 1024 - 2)) {
			index = i;
			break;
		} else if ( i > 1024 - 128 ) {
			index = 0;
		}
	}
	for ( uint16_t i = 0; i < 128; i++ ){
		yPos = map( adcBuffPTR[ i + index ], 0, 255, 0, 31 );
//		graphic_draw_V_line_RAM( buffer, i, 0, 31, BlackColor, &GammaRGB );
		graphic_set_pixel_RAM( buffer, i, yPos, &ColorRGBWhite );
	}
}
/***********************************************************************************/
#endif
/***************************** Obliczanie FFT oraz wyliczenie słupków ******************/
 void spectrum_calculate_fft( void ) {
	gpio_pin_HI( DEBUG_PORT0, DEBUG_PIN0 );	// DEBUG

#if defined FFT_FLOAT || defined FFT_CFFT
	sw_fft_calculate();
#else
	cqt();
#endif
	fftCalculate = 1;
	gpio_pin_LO( DEBUG_PORT0, DEBUG_PIN0 );	// DEBUG

#ifdef RFFT_U15
	arm_q15_to_float( bufferMagPTR, bufferMag, SAMPLES/2 );
#endif
//	gpio_pin_HI( DEBUG_PORT0, DEBUG_PIN0 );	// DEBUG
	calculate_bars( struct_pointer_ret()->gainSet );
//	gpio_pin_LO( DEBUG_PORT0, DEBUG_PIN0 );	// DEBUG
//	gpio_pin_HI( DEBUG_PORT0, DEBUG_PIN0 );	// DEBUG
#ifdef WAVE_DRAW
	static uint8_t cnt = 0;
	if ( cnt++%1 == 0 ) {
		draw_wave( TextBuffer );
		sw_update_screen();
	}
#endif
	fftCalculate = 0;
	calculateEndFftFlag = 1;
//	gpio_pin_LO( DEBUG_PORT0, DEBUG_PIN0 );	// DEBUG
}
/***************************************************************************************/
/********************************************* Funkcje do rysowania słupków  *******************************************/
 INLINE uint32_t count_position( T_BAR * bars, uint32_t lastPos, uint32_t barThick, uint32_t barGap  ) {
	return bars->yStart + 1 - (lastPos * (barThick + barGap) + 2 * barGap);
}
/*************************** Funkcja rysująca pasek postępu w pamięci RAM ******************************/
 void draw_Vbar_RAM( T_BAR *bar, T_SPECTRUM * SpectrumSets, uint8_t riseTimer, uint8_t fallTimer, uint8_t peakTimer ) {
	uint32_t width = SpectrumSets->barWidth;
	uint32_t yPos;
	uint32_t color = 0;
	//---------------------- Drawing bars in up direction ---------------------------------------
	if ( bar->lastHeight < bar->height && riseTimer==0 ) {
		bar->lastHeight++;				// Obliczamy nową wysokość wyświetlanego słupka
		yPos = count_position( bar, bar->lastHeight, SpectrumSets->barThick, SpectrumSets->barGap  );
		color = color_interpolate( SpectrumColors.barMin, SpectrumColors.barMax, (31-yPos), 31);

		graphic_fill_rectangle_RAM( SpectrumBuffer, bar->xStart, yPos, width, SpectrumSets->peakThick, color, &GammaRGB );

		// Rysujemy wartość szczytową, o jedną pozycję wyżej niż aktualna wysokość słupka
		if ( bar->peakHeight < bar->lastHeight + 1 ) {
			bar->peakHeight++;
			yPos = count_position( bar, bar->peakHeight, SpectrumSets->barThick, SpectrumSets->barGap );
			graphic_fill_rectangle_RAM( SpectrumBuffer, bar->xStart, yPos , width,
										SpectrumSets->peakThick, SpectrumColors.barPeak, &GammaRGB );
		}
	}
//---------------------- Drawing bars in down direction ----------------------------------------
	if ( bar->lastHeight > bar->height	&& fallTimer==0 ) {
		// Obliczamy pozycję piksela na podstawie wysokości słupka oraz ....
		yPos = count_position( bar, bar->lastHeight, SpectrumSets->barThick, SpectrumSets->barGap );
		graphic_fill_rectangle_RAM( SpectrumBuffer, bar->xStart, yPos, width,
									SpectrumSets->barThick, (uint32_t)0, &GammaRGB );
		bar->lastHeight--;				// Obliczamy nową wysokość wyświetlanego słupka
	}
// --------------------- Drawing peak indicator ---------------------
	if ( bar->peakHeight > bar->lastHeight + 1 && peakTimer==0 ) {
		yPos = count_position( bar, bar->peakHeight, SpectrumSets->barThick, SpectrumSets->barGap );
		graphic_fill_rectangle_RAM( SpectrumBuffer, bar->xStart,  yPos,     width, SpectrumSets->peakThick, (uint32_t)0, &GammaRGB ); 			// Kasujemy aktualny
		graphic_fill_rectangle_RAM( SpectrumBuffer, bar->xStart,  yPos + SpectrumSets->barThick + SpectrumSets->barGap, width,
									SpectrumSets->peakThick, SpectrumColors.barPeak, &GammaRGB ); // Rysujemy nowy
		bar->peakHeight--;
	}
}
 void light_intense_RAM( T_BAR *bar, uint8_t lightCNT, uint8_t riseTimer, uint8_t fallTimer ) {

	//---------------------- Drawing bars in up direction ---------------------------------------
	if ( bar->lastHeight < bar->height && riseTimer==0 ) {
		*set_get_bright2( lightCNT ) = (uint8_t)map( bar->lastHeight++, 0, 31, 0, 23 );
	}
//---------------------- Drawing bars in down direction ----------------------------------------
	if ( bar->lastHeight > bar->height	&& fallTimer==0 ) {
		*set_get_bright2( lightCNT ) = (uint8_t)map( bar->lastHeight--, 0, 31, 0, 23 );
	}

}
/*******************************************************************************************************/

/*********************************** Rysuje spectrum w pamięci RAM *************************************/
 void draw_spectrum_RAM( uint8_t barCnt, T_SETTINGS *settingsSTR ) {
	draw_Vbar_RAM( &SpectrumBarsAVG, &SpectrumSetsAVG, riseTimer, fallAvgTimer, peakAvgTimer );
	for ( uint8_t i = 0; i < barCnt;  i++) {
		if ( SpectrumBarsMAIN[i].barDir > 0)
			SpectrumBarsMAIN[i].barDir--;
		draw_Vbar_RAM( SpectrumBarsMAIN + i, &SpectrumSetsMAIN, riseTimer, fallTimer, peakTimer );
	}
	// ------------------------------------------------
	if ( riseTimer == 0 )
		riseTimer = 1 * settingsSTR->riseTimeSet;	// Startujemy licznik opadania znacznika wartości szczytowej
	if ( fallTimer == 0 )
		fallTimer = 1 * settingsSTR->fallTimeSet;	// Startujemy licznik opadania słupka
	if ( peakTimer == 0 )
		peakTimer = 1 * settingsSTR->peakTimeSet;	// Startujemy licznik opadania znacznika wartości szczytowej
	if ( fallAvgTimer == 0 )
		fallAvgTimer = 1 * settingsSTR->fallAvgTimeSet;	// Startujemy licznik opadania słupka
	if ( peakAvgTimer == 0 )
		peakAvgTimer = 1 * settingsSTR->peakAvgTimeSet;	// Startujemy licznik opadania znacznika wartości szczytowej
}
/*******************************************************************************************************/
 void lights_spectrum_RAM( T_SETTINGS *settingsSTR ) {
	light_intense_RAM( SpectrumBarsMAIN + 1,  0, riseTimer, fallTimer );
	light_intense_RAM( SpectrumBarsMAIN + 8,  1, riseTimer, fallTimer );
	light_intense_RAM( SpectrumBarsMAIN + 12, 2, riseTimer, fallTimer );

	if ( riseTimer == 0 )
		riseTimer = 1 * settingsSTR->riseTimeSet;	// Startujemy licznik opadania znacznika wartości szczytowej
	if ( fallTimer == 0 )
		fallTimer = 1 * settingsSTR->fallTimeSet;	// Startujemy licznik opadania słupka
}
/*******************************************************************************************************/

/***************************************************************************************/
 INTERRUPT void TIM7_IRQHandler(void) {
	if ( TIM7->SR & TIM_SR_UIF ) {
		TIM7->SR &= ~TIM_SR_UIF;
	//------- Sekcja timera ---------
		volatile register uint32_t n;
		n = riseTimer;
		if (n>0) riseTimer = --n;
		n = fallTimer;
		if (n>0) fallTimer = --n;
		n = peakTimer;
		if (n>0) peakTimer = --n;

		n = fallAvgTimer;
		if (n>0) fallAvgTimer = --n;
		n = peakAvgTimer;
		if (n>0) peakAvgTimer = --n;

		if (fftCalculate == 1 )
			return;
	//-------------------------------
		if ( *set_driver_mode() == DRIVER_MODE_1 )
			draw_spectrum_RAM( barsCNT, struct_pointer_ret() );
		else
			lights_spectrum_RAM( struct_pointer_ret() );
	}
}
/*************************************************************************************/

/******** Ustawia maksymalną wysokość słupków *******/
void setHeightBar( uint8_t barHeightMax ) {
	SpectrumSetsMAIN.barHeightMax = barHeightMax;
}
/****************************************************/

/************************ Funkcje callback i zdarzeniowe *****************************/
static void ( *calculateEnd_fft_event_callback )( void );
void register_calculateEnd_fft_event_callback( void (*callback)( void ) ) {
	calculateEnd_fft_event_callback = callback;
}
//------------------------------------------------------------------------------------
void SW_CALCULATE_END_FFT_EVENT( void ) {
	if ( calculateEndFftFlag ) {
		if( calculateEnd_fft_event_callback ) {
			calculateEnd_fft_event_callback();
		}
		calculateEndFftFlag = 0;
	}
}

static void ( *drawSpectrumEnd_event_callback )( void );
void register_drawSpectrumEnd_event_callback( void (*callback)( void ) ) {
	drawSpectrumEnd_event_callback = callback;
}
//------------------------------------------------------------------------------------
void SW_DRAW_SPECTRUM_END_EVENT( void ) {
	if ( drawSpectrumFlag ) {
		if( drawSpectrumEnd_event_callback ) {
			drawSpectrumEnd_event_callback();
		}
		drawSpectrumFlag = 0;
	}
}

/*************************************************************************************/
//------------------------------------------------------------------------------------------------


