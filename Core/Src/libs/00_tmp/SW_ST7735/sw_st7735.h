/*
 * sw_st7735.h
 *
 *  Created on: 21 sie 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_SW_GRAPHICS_SW_ST7735_SW_ST7735_H_
#define SRC_LIBS_SW_GRAPHICS_SW_ST7735_SW_ST7735_H_

// Screen resolution in normal orientation
#define ST7735_WIDTH         128
#define ST7735_HEIGHT        160

#define DC_PORT			PORTA
#define DC_PIN			PA0
#define RST_PORT		PORTA
#define RST_PIN			PA0

#define ST7735_SWRESET	0x01
#define ST7735_SLPOUT	0x11
#define ST7735_NORON	0x13
#define ST7735_INVOFF	0x20
#define ST7735_INVON	0x21
#define ST7735_DISPOFF	0x28
#define ST7735_DISPON	0x29
#define ST7735_CASET	0x2A
#define ST7735_RASET	0x2B
#define ST7735_RAMWR	0x2C
#define ST7735_COLMOD	0x3A
#define ST7735_MADCTL	0x36

#define ST7735_FRMCTR1	0xB1
#define ST7735_FRMCTR2	0xB2
#define ST7735_FRMCTR3	0xB3
#define ST7735_INVCTR	0xB4
#define ST7735_GAMSET	0x26

typedef enum {
	scr_normal = 0,
	scr_CW     = 1,
	scr_CCW    = 2,
	scr_180    = 3
} ScrOrientation_TypeDef;



extern void ST7735_write( uint8_t data );

extern uint16_t RGB565( uint8_t R,uint8_t G,uint8_t B );

extern void ST7735_Init( void );
extern void ST7735_AddrSet( uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE );
extern void ST7735_Orientation( uint8_t orientation );
extern void ST7735_Clear( uint16_t color );

extern void ST7735_Pixel( uint16_t X, uint16_t Y, uint16_t color );
extern void ST7735_HLine( uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color );
extern void ST7735_VLine( uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color );
extern void ST7735_Line( int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color );
extern void ST7735_Rect( uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color );
extern void ST7735_FillRect( uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color );


#endif /* SRC_LIBS_SW_GRAPHICS_SW_ST7735_SW_ST7735_H_ */
