/*
 * colors.h
 *
 *  Created on: 25.11.2018
 *      Author: simon
 */

#ifndef LIBS_SW_GRAPHICS_COLORS_H_
#define LIBS_SW_GRAPHICS_COLORS_H_

#include "math.h"


#ifndef float32_t
	typedef float float32_t;
#endif


/****** Definicje kolorów w pamięci Flash *****/
typedef struct {
	float32_t  gammaR, gammaG, gammaB;
} T_GAMMA;


/****** Definicje kolorów w pamięci Flash VERSJA 3 *****/
#define maroon 				(uint32_t)0x800000
#define dark_red 			(uint32_t)0x8B0000
#define brown 				(uint32_t)0xA52A2A
#define firebrick	 		(uint32_t)0xB22222
#define crimson				(uint32_t)0xDC143C
#define red 				(uint32_t)0xFF0000
#define tomato 				(uint32_t)0xFF6347
#define coral 				(uint32_t)0xFF7F50
#define indian_red 			(uint32_t)0xCD5C5C
#define light_coral 		(uint32_t)0xF08080
#define dark_salmon 		(uint32_t)0xE9967A
#define salmon 				(uint32_t)0xFA8072
#define light_salmon 		(uint32_t)0xFFA07A
#define orange_red 			(uint32_t)0xFF4500
#define dark_orange 		(uint32_t)0xFF8C00
#define orange 				(uint32_t)0xFFA500
#define gold 				(uint32_t)0xFFD700
#define dark_golden_rod		(uint32_t)0xB8860B
#define golden_rod 			(uint32_t)0xDAA520
#define pale_golden_rod 	(uint32_t)0xEEE8AA
#define dark_khaki 			(uint32_t)0xBDB76B
#define khaki 				(uint32_t)0xF0E68C
#define olive 				(uint32_t)0x808000
#define yellow				(uint32_t)0xFFFF00
#define yellow_green 		(uint32_t)0x9ACD32
#define dark_olive_green	(uint32_t)0x556B2F
#define olive_drab	 		(uint32_t)0x6B8E23
#define lawn_green 			(uint32_t)0x7CFC00
#define chart_reuse		 	(uint32_t)0x7FFF00
#define green_yellow 		(uint32_t)0xADFF2F
#define dark_green 			(uint32_t)0x006400
#define green	 			(uint32_t)0x008000
#define light_grey			(uint32_t)0x101010
#define dark_grey			(uint32_t)0x010101

//const uint32_t * Palette_32[32] (uint32_t){
//	(const uint32_t *)&maroon,
//	(const uint32_t *)&dark_red,
//	(const uint32_t *)&brown,
//	(const uint32_t *)&firebrick,
//	(const uint32_t *)&crimson,
//	(const uint32_t *)&red,
//	(const uint32_t *)&tomato,
//	(const uint32_t *)&coral,
//	(const uint32_t *)&indian_red,
//	(const uint32_t *)&light_coral,
//	(const uint32_t *)&dark_salmon,
//	(const uint32_t *)&salmon,
//	(const uint32_t *)&light_salmon,
//	(const uint32_t *)&orange_red,
//	(const uint32_t *)&dark_orange,
//	(const uint32_t *)&orange,
//	(const uint32_t *)&gold,
//	(const uint32_t *)&dark_golden_rod,
//	(const uint32_t *)&golden_rod,
//	(const uint32_t *)&pale_golden_rod,
//	(const uint32_t *)&dark_khaki,
//	(const uint32_t *)&khaki,
//	(const uint32_t *)&olive,
//	(const uint32_t *)&yellow,
//	(const uint32_t *)&yellow_green,
//	(const uint32_t *)&dark_olive_green,
//	(const uint32_t *)&olive_drab,
//	(const uint32_t *)&lawn_green,
//	(const uint32_t *)&chart_reuse,
//	(const uint32_t *)&green_yellow,
//	(const uint32_t *)&dark_green,
//	(const uint32_t *)&green,
//}

#define BlackColor 		(uint32_t)0x000000
#define WhiteColor 		(uint32_t)0xFFFFFF
#define RedColor 		(uint32_t)0xff0000
#define LimeColor 		(uint32_t)0x00FF00
#define BlueColor 		(uint32_t)0x0000FF
#define YellowColor		(uint32_t)0xFFFF00
#define CyanColor	 	(uint32_t)0x00FFFF
#define MagentaColor	(uint32_t)0xFF00FF
#define SilverColor 	(uint32_t)0xC0C0C0
#define GrayColor 		(uint32_t)0x808080
#define MaroonColor		(uint32_t)0x800000
#define OliveColor 		(uint32_t)0x808000
#define GreenColor 		(uint32_t)0x008000
#define PurpleColor 	(uint32_t)0x800080
#define TealColor 		(uint32_t)0x008080
#define NavyColor 		(uint32_t)0x000080

//#define * Palette_16[16] (uint32_t){
//	(const uint32_t *)&BlackColor,
//	(const uint32_t *)&WhiteColor,
//	(const uint32_t *)&RedColor,
//	(const uint32_t *)&LimeColor,
//	(const uint32_t *)&BlueColor,
//	(const uint32_t *)&YellowColor,
//	(const uint32_t *)&CyanColor,
//	(const uint32_t *)&MagentaColor,
//	(const uint32_t *)&SilverColor,
//	(const uint32_t *)&GrayColor,
//	(const uint32_t *)&MaroonColor,
//	(const uint32_t *)&OliveColor,
//	(const uint32_t *)&GreenColor,
//	(const uint32_t *)&PurpleColor,
//	(const uint32_t *)&TealColor,
//	(const uint32_t *)&NavyColor,
//}

#ifdef PANEL_RGB
static const T_RGB_2ROWS ColorRGBWhite = {0x3f};
static const T_RGB_2ROWS BlackColorTRGB[] = {
	{0},{0},{0},{0},
	{0},{0},{0},{0},
};
#endif

static const T_GAMMA GammaRGB = {
	1, 		// R
	1,		// G
	1		// B
};

/************************* Setting gamma to single color *********************************/
static inline uint8_t sw_graphic_gamma( uint8_t color, float32_t gamma ) {
	uint8_t tmp = (uint8_t )255 * powf( (float32_t)color/255, (float32_t)(gamma) );
	return tmp;
}
/*****************************************************************************************/


#endif /* LIBS_SW_GRAPHICS_COLORS_H_ */
