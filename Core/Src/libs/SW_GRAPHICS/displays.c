/*
 * displays.c
 *
 *  Created on: 5 lut 2017
 *      Author: simon
 */

#include "sw_mcu_conf.h"

#include "SW_SSD1306/framebuffer.h"
#include "SW_SSD1306/sw_ssd1306_v1.h"
#include "displays.h"


/***************************************************************************************/
void graphic_draw_H_line_RAM( volatile T_DISPLAY *buffer, int16_t x, int16_t y, uint8_t width,
								  const uint32_t color, const T_GAMMA *gamma ) {

#ifdef PANEL_RGB
	T_RGB_2ROWS colorTmp[ 8 ];
	graphic_24bit_to_rgb( color, colorTmp, gamma );
#endif
	for ( uint8_t i=0; i < width; i++ ) {
#ifdef PANEL_RGB
		graphic_set_pixel_RAM( buffer, x + i, y, colorTmp );
#elif defined COLOR_MONO
		graphic_set_pixel_RAM( buffer, x + i, y, (uint8_t *)&color );
#endif
	}
}
/***************************************************************************************/

/***************************************************************************************/
void graphic_draw_V_line_RAM( volatile T_DISPLAY *buffer, int16_t x, int16_t y, uint8_t height,
							  const uint32_t color, const T_GAMMA *gamma ) {
#ifdef PANEL_RGB
	T_RGB_2ROWS colorTmp[ 8 ];
	graphic_24bit_to_rgb( color, colorTmp, gamma );
#endif

	for ( uint8_t i=0; i < height; i++ ) {
#ifdef PANEL_RGB
		graphic_set_pixel_RAM( buffer, x, y+i, colorTmp );
#elif defined COLOR_MONO
		graphic_set_pixel_RAM( buffer, x, y+i, (uint8_t *)&color );
#endif
	}
}
/***************************************************************************************/

/***************************************************************************************/
void graphic_fill_rectangle_RAM( volatile T_DISPLAY *buffer, int16_t xPos, int16_t yPos, uint8_t width,
								 uint8_t height, const uint32_t color, const T_GAMMA *gamma ) {
	for ( uint8_t i=0; i < height; i++ ) {
		graphic_draw_H_line_RAM( buffer, xPos, yPos+i, width, color, gamma );
	}
}
/***************************************************************************************/

/***************************************************************************************/
void graphic_draw_rectangle_RAM( volatile T_DISPLAY *buffer, int16_t xPos, int16_t yPos, uint8_t width,
								 uint8_t height, const uint32_t color, const T_GAMMA *gamma ) {
	graphic_draw_H_line_RAM( buffer, xPos, yPos, 		width, color, gamma );
	graphic_draw_H_line_RAM( buffer, xPos, yPos+height-1, width, color, gamma );

	graphic_draw_V_line_RAM( buffer, xPos,			yPos, height, color, gamma );
	graphic_draw_V_line_RAM( buffer, xPos+width-1,	yPos, height, color, gamma );
}
/***************************************************************************************/

/***************************************************************************************/
void graphic_fill_circle( volatile T_DISPLAY * buffer, int16_t xs, int16_t ys, int16_t r, uint32_t color ) {
	int16_t x = 0, y = r;
	int16_t e = 0, e1, e2;

	while (x <= y) {
		graphic_draw_H_line_RAM( buffer, -y + xs,  x + ys, (y << 1) + 1, color, &GammaRGB );
		graphic_draw_H_line_RAM( buffer, -y + xs, -x + ys, (y << 1) + 1, color, &GammaRGB );

		e1 = e + (x << 1) + 1;
		e2 = e1 - (y << 1) + 1;
		if  (e1 + e2 >= 0 ) {
			graphic_draw_H_line_RAM( buffer, -x + xs,  y + ys, (x << 1) + 1, color, &GammaRGB );
			graphic_draw_H_line_RAM( buffer, -x + xs, -y + ys, (x << 1) + 1, color, &GammaRGB );
			y--;
			e = e2;
		} else
			e = e1;
		x++;
	}
}
/***************************************************************************************/

/**************************** Interpolate range of colors *******************************/
uint32_t color_interpolate( uint32_t minColor, uint32_t maxColor, uint8_t pos, uint8_t posMax ) {
	T_RGB ColorMin, ColorMax;
	ColorMin.color24bit = minColor;
	ColorMax.color24bit = maxColor;

	ColorMin.R = (uint8_t)ColorMin.R + ( (uint16_t)ColorMax.R - (uint16_t)ColorMin.R  )*pos / posMax;
	ColorMin.G = (uint8_t)ColorMin.G + ( (uint16_t)ColorMax.G - (uint16_t)ColorMin.G  )*pos / posMax;
	ColorMin.B = (uint8_t)ColorMin.B + ( (uint16_t)ColorMax.B - (uint16_t)ColorMin.B  )*pos / posMax;

	return ColorMin.color24bit;
}
/***************************************************************************************/

/************************************ Works in the loop ************************************/
void graphic_color_test( uint8_t onlyOnce, volatile T_DISPLAY * buffer ) {
	static uint8_t i 	= 0;
	static uint8_t draw = 0;

	uint16_t w = SCREEN_WIDTH - 1;
	uint16_t h = SCREEN_HEIGHT;
//	uint16_t a = 256 / SCREEN_WIDTH;
	uint16_t b = h / 4;
//	uint32_t Color = 0UL;
	uint32_t Color = 0xffffffff;

	if ( !draw ) {
//		Color = color_interpolate( CyanColor, RedColor,  i, SCREEN_WIDTH );
		graphic_fill_rectangle_RAM( buffer, i,   0, 1, b, Color, &GammaRGB );

//		Color = color_interpolate( BlueColor, RedColor,  i, SCREEN_WIDTH );
		graphic_fill_rectangle_RAM( buffer, w-i, 1*b, 1, b, Color, &GammaRGB );

//		Color = color_interpolate( GreenColor, RedColor,  i, SCREEN_WIDTH );
		graphic_fill_rectangle_RAM( buffer, i, 2*b, 1, b, Color, &GammaRGB );

//		Color = color_interpolate( CyanColor, YellowColor,  i, SCREEN_WIDTH );
		graphic_fill_rectangle_RAM( buffer, w-i, 3*b, 1, b, Color, &GammaRGB );
		i++;
		if ( i == SCREEN_WIDTH ) {
			if (onlyOnce) return;					// Rysujemy tylko raz
			i = 0;	draw = 1;
		}
	} else {
		graphic_draw_V_line_RAM( buffer, i,		0,		b, BlackColor, &GammaRGB );
		graphic_draw_V_line_RAM( buffer, w - i,	b,		b, BlackColor, &GammaRGB );
		graphic_draw_V_line_RAM( buffer, i,		2*b,	b, BlackColor, &GammaRGB );
		graphic_draw_V_line_RAM( buffer, w - i,	3*b,	b, BlackColor, &GammaRGB );
		i++;
		if ( i == SCREEN_WIDTH ) {
			if (onlyOnce) return;					// Rysujemy tylko raz
			i = 0;	draw = 0;
		}
	}
}
/*******************************************************************************************/

/***************************************************************************************/
void graphic_floatingLine_test(volatile T_DISPLAY *buffer, const uint32_t color24bit ) {
	static int16_t iX = 0, iY = 0;
	static uint16_t dirX = 1, dirY = 1;

	graphic_draw_V_line_RAM( buffer, iX, 0, SCREEN_HEIGHT, color24bit, &GammaRGB );
//	graphic_draw_H_line_RAM( buffer, 0, iY, SCREEN_WIDTH,  color24bit, &GammaRGB );
	graphic_draw_V_line_RAM( buffer, iX, 0, 1, GreenColor, &GammaRGB );
	graphic_draw_V_line_RAM( buffer, iX, 16, 1, RedColor, &GammaRGB );

	if (iX == SCREEN_WIDTH - 1)		dirX = 0;
	if (iX == 0) 					dirX = 1;
	if (iY == SCREEN_HEIGHT - 1)	dirY = 0;
	if (iY == 0) 					dirY = 1;

	if (dirX == 1) iX++;
	else iX--;
	if (dirY == 1) iY++;
	else iY--;
}
/***************************************************************************************/



