#ifndef DISPLAYS_H_
#define DISPLAYS_H_

#include "colors.h"


#ifndef float32_t
	typedef float float32_t;
#endif


void graphic_draw_H_line_RAM( volatile T_DISPLAY *buffer, int16_t x, int16_t y, uint8_t width,
								  const uint32_t color, const T_GAMMA * gamma );
void graphic_draw_V_line_RAM( volatile T_DISPLAY *buffer, int16_t x, int16_t y, uint8_t height,
							  const uint32_t color, const T_GAMMA * gamma );

void graphic_draw_rectangle_RAM( volatile T_DISPLAY *buffer, int16_t xPos, int16_t yPos, uint8_t width,
								  uint8_t height, const uint32_t color, const T_GAMMA * gamma );
void graphic_fill_rectangle_RAM( volatile T_DISPLAY *buffer, int16_t xPos, int16_t yPos, uint8_t width,
								 uint8_t height, const uint32_t color, const T_GAMMA *gamma );
void graphic_fill_circle( volatile T_DISPLAY * buffer, int16_t xs, int16_t ys, int16_t r, uint32_t color );

// Funkcje oparte na rysowaniu pojedynczego piksela
void graphic_24bit_to_rgb ( const uint32_t color24bit, T_RGB_2ROWS *color, const T_GAMMA * gamma );

void graphic_draw_sqr_pixel( volatile T_DISPLAY *buffer, int16_t x, int16_t y, const uint32_t color, const T_GAMMA * gamma );

void sw_generate_colors( uint8_t onlyOnce, T_DISPLAY * buffer );
uint32_t color_interpolate( uint32_t minColor, uint32_t maxColor, uint8_t pos, uint8_t posMax );

void graphic_color_test( uint8_t onlyOnce, volatile T_DISPLAY * buffer );

uint32_t *  PALETTE_set_colors (void);

void graphic_pixel_test( volatile T_DISPLAY *buffer, const uint32_t color24bit );
void graphic_floatingLine_test(volatile T_DISPLAY *buffer, const uint32_t color24bit );

void sw_graphic_clear_line( volatile T_DISPLAY * buffer, uint8_t line );


#endif /* LIBS_SW_GP9002_DISPLAYS_H_ */
