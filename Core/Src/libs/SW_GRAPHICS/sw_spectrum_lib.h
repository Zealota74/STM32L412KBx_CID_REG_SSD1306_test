/*
 * spectrum_lib.h
 *
 *  Created on: 25 lut 2016
 *      Author: simon
 */

#ifndef LIBS_SW_GRAPHICS_SW_SPECTRUM_LIB_H_
#define LIBS_SW_GRAPHICS_SW_SPECTRUM_LIB_H_


#define WAVE_DRAW

#define TABLE_COEF_512	(uint32_t)44
#define TABLE_COEF_256	(uint32_t)33
#define TABLE_COEF_128	(uint32_t)31

//#define BARS_CNT_MAX			(uint32_t)8 //SCREEN_WIDTH/2		// Ilość słupków na ekranie
//#define BARS_CNT_MAX			(uint32_t)16 //SCREEN_WIDTH/2		// Ilość słupków na ekranie
//#define BARS_CNT_MAX			(uint32_t)23 //SCREEN_WIDTH/2		// Ilość słupków na ekranie
#define BARS_CNT_MAX			(uint32_t)64 //SCREEN_WIDTH/2		// Ilość słupków na ekranie
//#define BARS_CNT_MAX			TABLE_COEF_512 //SCREEN_WIDTH/2		// Ilość słupków na ekranie
//#define BARS_CNT_MAX			TABLE_COEF_256 //SCREEN_WIDTH/2		// Ilość słupków na ekranie
//#define BARS_CNT_MAX			TABLE_COEF_128 //SCREEN_WIDTH/2		// Ilość słupków na ekranie

#define SPECTRUM_MODE_BARS			0
#define SPECTRUM_MODE_COLOROFON		1


/********* STAŁE pomocnicze ***********/
#define BAR_UP		-1
#define BAR_DOWN	-2
/**************************************/


// Parametry całego spektrum --------------
#define Y_START		30			// Wysokość na jakiej zaczynają się słupki, aktualnie 30 piksel

/************** Struktura definiują parametry pojedynczego słupka *************************************/
typedef struct {
	uint8_t xStart;			// Początkowo pozycja słupka w poziomie
	uint8_t yStart;			// Początkowo pozycja słupka w poziomie
	uint8_t lastHeight;		// Aktualna wysokość słupka dążąca do wartości wyliczonej
	uint8_t height;			// Aktualnie ostatnio wyliczona wysokość słupka
	uint8_t peakHeight;		// Aktualna wysokość piku

	int16_t barDir;			// Kierunek ruchu słupka oraz licznik czasu zatrzymania wartości szczytowej
} T_BAR;
/**********************************************************/
enum {
	defaultColor, altColor1, altColor2, altColor3, altColor4, altColor5,
	altColor6, altColor7, altColor8
};
typedef struct {
	uint32_t barMin, barMid, barMax, barPeak;
} T_COLORS;

enum {
	barContinuos, barDot, onlyPeak
};

/************* Struktura definiująca spektrum *************/
typedef struct {
	uint8_t barType;
	uint8_t barWidth, barThick, barGap;
	uint8_t barsCNT;
	uint8_t peakWidth, peakThick, peakJump;
	uint8_t barHeightMax;
} T_SPECTRUM;
/**********************************************************/

extern volatile uint8_t calculate;
extern volatile uint8_t drawSpectrumFlag;

extern volatile bool fftCalculate;

extern void sw_spectrum_init( uint8_t spectrumMode );

extern void clear_spectrum_RAM( void );
extern void set_spectrum_param( uint8_t mode );
extern void spectrum_colors_init( uint8_t cnt ) ;

extern void spectrum_calculate_fft( void );

extern void setHeightBar( uint8_t barHeightMax );

extern void sw_visualisation_mode( void );

// Event and callback functions -----------------------------------------------------------------------
extern void SW_CALCULATE_END_FFT_EVENT( void );
extern void register_calculateEnd_fft_event_callback( void (*callback)( void ) );

extern void SW_DRAW_SPECTRUM_END_EVENT( void );
extern void register_drawSpectrumEnd_event_callback( void (*callback)( void ) );

extern void set_spectrum_colors( uint8_t colors );

int freqToIndex(int freq);
void calcBands(void);
void calcAmplitudes();

static const uint32_t TablicaIter[44] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13,
	15, 17, 19, 22, 24, 28, 31, 35, 39, 44, 50, 56, 63, 71, 79, 89, 101,
	113, 127, 143, 160, 180, 202, 227, 255, 286, 322, 361, 405, 455, 511,
};
static const float32_t A_Veight_128[31] = {
	0.123174, 0.292589, 0.433728, 0.546524, 0.634572, 0.702407, 0.754425,
	0.794341, 0.825071, 0.848817, 0.881492, 0.901011, 0.912187, 0.917791,
	0.919492, 0.916908, 0.912675, 0.903555, 0.887524, 0.868271, 0.846685,
	0.817379, 0.780022, 0.741427, 0.696075, 0.645142, 0.596241, 0.53356,
	0.476905, 0.422139, 0.367024,
};
static const T_COLORS DefaultColors = {
	GreenColor, YellowColor, RedColor, CyanColor
};
static const T_COLORS AltColor1 = {
	RedColor, YellowColor, orange, CyanColor
};
static const T_COLORS AltColor2 = {
	BlueColor, YellowColor, MagentaColor, RedColor
};
static const T_COLORS AltColor3 = {
	RedColor, CyanColor, BlueColor, YellowColor
};
static const T_COLORS AltColor4 = {
	BlackColor, GrayColor, BlackColor, CyanColor
};
static const T_COLORS AltColor5 = {
	coral, BlueColor, TealColor/2, RedColor
};
static const T_COLORS AltColor6 = {
	YellowColor, orange, RedColor, CyanColor
};


#define GST_DLBFFT_BINS			64
#define GST_DLBFFT_SLICE_RATE	64

#define GST_DLBFFT_SCALE		0.05f
#define GST_DLBFFT_SMOOTHING	0.00007f
#define GST_DLBFFT_GAMMA		2.0f


#endif /* LIBS_SW_GRAPHICS_SW_SPECTRUM_LIB_H_ */
