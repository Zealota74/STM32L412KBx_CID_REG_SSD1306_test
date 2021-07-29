/*
 * framebuffer.h
 *
 *  Created on: 12.08.2018
 *      Author: simon
 */
#ifndef LIBS_FRAMEBUFFER_H_
#define LIBS_FRAMEBUFFER_H_


#include "brightness.h"


#define COLOR_MONO
//#define PANEL_RGB

#define FRAMEBUFFER_MAXBITDEPTH	1

#ifdef PANEL_RGB
// Single panel parameters
#	define PANEL_WIDTH				64
#	define PANEL_HEIGHT				32
// Number of panels, per display
#	define PANELS					2
#	define PANEL_ROWS_CNT			16
#	define PANEL_ROW_SCAN			16
#	define FRAMEBUFFER_BUFFERS		1
#	define SCREEN_HEIGHT			PANEL_HEIGHT
#	define SCREEN_WIDTH				( (uint32_t)(PANELS * PANEL_WIDTH) )
#	define FRAMEBUFFER_ALL			(uint32_t)FRAMEBUFFER_BITSPERLINE * FRAMEBUFFER_MAXBITDEPTH * PANEL_HEIGHT/2
//Number of bits to clock out each time
#	define FRAMEBUFFER_BITSPERLINE	(uint32_t)( SCREEN_WIDTH )
#elif defined COLOR_MONO
#	define SCREEN_WIDTH				128
#	define SCREEN_HEIGHT			64
#	define FRAMEBUFFER_BITSPERLINE	SCREEN_WIDTH
#	define FRAMEBUFFER_ALL			(uint32_t)SCREEN_WIDTH * SCREEN_HEIGHT/8
#endif

extern volatile uint8_t sync_flag1, sync_flag2;

extern volatile uint8_t refreshScreen;



/************* Unia przygotowana dla wyświetlaczy RGB ***************/
typedef union {
	uint8_t rows_0_31;
	struct {
		uint8_t rows_0_15: 3;
		uint8_t rows_16_31: 3;
//		uint8_t tmp: 2;
	};
}T_RGB_2ROWS;

typedef struct {
	T_RGB_2ROWS Buffer0;
	T_RGB_2ROWS Buffer1;
}T_RGB_Buffers;

/*********************************************************************/
typedef union {
	uint32_t color24bit;
	struct {		// bits
		uint8_t B;	// 7 - 0
		uint8_t G;	// 15 - 8
		uint8_t R;	// 16 - 24
		uint8_t tmp;
	};
} T_RGB;

//typedef T_RGB_2ROWS		T_DISPLAY;
typedef uint8_t			T_DISPLAY;
//typedef T_RGB_Buffers	T_DISPLAY;

#ifdef PANEL_RGB
typedef T_RGB			T_COLOR;
#elif defined COLOR_MONO
typedef uint8_t			T_COLOR;
#endif


extern volatile T_DISPLAY * SpectrumBuffer;

extern volatile T_DISPLAY * TextBufferProgram;
extern volatile T_DISPLAY * TextBufferDisplay;

extern volatile T_DISPLAY * TextBuffer;

extern T_DISPLAY * Displays[3];

void clear_buffer_RAM( volatile T_DISPLAY * buffer );

void sw_update_screen( void );



#endif /* LIBS_SW_GRAPHICS_FRAMEBUFFER_H_ */
