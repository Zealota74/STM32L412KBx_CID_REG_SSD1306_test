/*
 * sw_st7735.c
 *
 *  Created on: 21 sie 2021
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "../../SW_BOARD/sw_gpio.h"
#include "../../SW_TIMERS/sw_soft_timers.h"
#include "../../SW_STM_PERIPHS/sw_spi.v2.h"

#include "sw_st7735.h"

static uint16_t scr_width;
static uint16_t scr_height;


/************************** Base functions ***************************/
static INLINE void DC_SET(void)  { gpio_pin_HI( DC_PORT, DC_PIN );}
static INLINE void DC_CLR(void)  { gpio_pin_LO( DC_PORT, DC_PIN );}
static INLINE void RST_SET(void) { gpio_pin_HI( RST_PORT, RST_PIN );}
static INLINE void RST_CLR(void) { gpio_pin_LO( RST_PORT, RST_PIN );}

static INLINE void ST7735_cmd( uint8_t cmd ) {
	DC_CLR();
	sw_spi_hw_send_buff( &cmd, 1, spi_8bit );		//
}
static INLINE void ST7735_data( uint8_t data ) {
	DC_SET();
	sw_spi_hw_send_buff( &data, 1, spi_8bit );		//
}
/*********************************************************************/

/*********************************************************************/
void ST7735_Init(void) {
	gpio_pin_cfg( DC_PORT, DC_PIN, gpio_mode_output_PP_HS );
	gpio_pin_cfg( RST_PORT, RST_PIN, gpio_mode_output_PP_HS );

	// Reset display
	RST_SET();
	delay_ms(5);
	RST_CLR();
	delay_ms(5);
	RST_SET();
	delay_ms(5);

	ST7735_cmd(ST7735_SLPOUT);		// Sleep out & booster on
	delay_ms(20);					// Datasheet says what display wakes about 120ms
									// (may be much faster actually)
	ST7735_cmd( ST7735_FRMCTR1 );	// In normal mode (full colors):
	ST7735_data(0x05);				// 	RTNA set 1-line period: RTNA2, RTNA0
	ST7735_data(0x3c);				// 	Front porch: FPA5,FPA4,FPA3,FPA2
	ST7735_data(0x3c);				// 	Back porch: BPA5,BPA4,BPA3,BPA2

	ST7735_cmd(ST7735_FRMCTR2);		// In idle mode (8-colors):
	ST7735_data(0x05);				//  RTNB set 1-line period: RTNAB, RTNB0
	ST7735_data(0x3c);				//  Front porch: FPB5,FPB4,FPB3,FPB2
	ST7735_data(0x3c);				//  Back porch: BPB5,BPB4,BPB3,BPB2

	ST7735_cmd(ST7735_FRMCTR3);		// In partial mode + full colors:
	ST7735_data(0x05);				//  RTNC set 1-line period: RTNC2, RTNC0
	ST7735_data(0x3c);				//  Front porch: FPC5,FPC4,FPC3,FPC2
	ST7735_data(0x3c);				//  Back porch: BPC5,BPC4,BPC3,BPC2
	ST7735_data(0x05);				//  RTND set 1-line period: RTND2, RTND0
	ST7735_data(0x3c);				//  Front porch: FPD5,FPD4,FPD3,FPD2
	ST7735_data(0x3c);				//  Back porch: BPD5,BPD4,BPD3,BPD2

	ST7735_cmd(ST7735_INVCTR);		// Display dot inversion control:
	ST7735_data(0x03);				//  NLB,NLC

	ST7735_cmd(ST7735_COLMOD);		// Interface pixel format
//	ST7735_data(0x03);				// 12-bit/pixel RGB 4-4-4 (4k colors)
	ST7735_data(0x05);				// 16-bit/pixel RGB 5-6-5 (65k colors)
//	ST7735_data(0x06);				// 18-bit/pixel RGB 6-6-6 (256k colors)

//	ST7735_cmd(ST7735_MADCTL);		// Memory data access control:
									//  MY MX MV ML RGB MH - -
//	ST7735_data(0x00);				//  Normal: Top to Bottom; Left to Right; RGB
//	ST7735_data(0x80);				//  Y-Mirror: Bottom to top; Left to Right; RGB
//	ST7735_data(0x40);				//  X-Mirror: Top to Bottom; Right to Left; RGB
//	ST7735_data(0xc0);				//  X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
//	ST7735_data(0x20);				//  X-Y Exchange: X and Y changed positions
//	ST7735_data(0xA0);				//  X-Y Exchange,Y-Mirror
//	ST7735_data(0x60);				//  X-Y Exchange,X-Mirror
//	ST7735_data(0xE0);				//  X-Y Exchange,X-Mirror,Y-Mirror

	ST7735_cmd(ST7735_INVOFF);		// Display inversion off
//	ST7735_cmd(ST7735_INVON);		// Display inversion on

	ST7735_cmd(ST7735_NORON);		// Partial mode off

	ST7735_cmd(ST7735_GAMSET);		// Gamma curve set:
	ST7735_data(0x01);				//  Gamma curve 1 (G2.2) or (G1.0)
//	ST7735_data(0x02);				//  Gamma curve 2 (G1.8) or (G2.5)
//	ST7735_data(0x04);				//  Gamma curve 3 (G2.5) or (G2.2)
//	ST7735_data(0x08);				//  Gamma curve 4 (G1.0) or (G1.8)

	ST7735_cmd(ST7735_DISPON);		// Display on
	ST7735_Orientation(scr_normal);
}

void ST7735_Orientation( ScrOrientation_TypeDef orientation ) {
	ST7735_cmd( ST7735_MADCTL );	// Memory data access control:
	switch( orientation ) {
		case scr_CW:
			scr_width  = ST7735_HEIGHT;
			scr_height = ST7735_WIDTH;
			ST7735_data(0xA0);		// X-Y Exchange,Y-Mirror
			break;
		case scr_CCW:
			scr_width  = ST7735_HEIGHT;
			scr_height = ST7735_WIDTH;
			ST7735_data(0x60);		// X-Y Exchange,X-Mirror
			break;
		case scr_180:
			scr_width  = ST7735_WIDTH;
			scr_height = ST7735_HEIGHT;
			ST7735_data(0xc0);		// X-Mirror,Y-Mirror: Bottom to top; Right to left; RGB
			break;
		default:
			scr_width  = ST7735_WIDTH;
			scr_height = ST7735_HEIGHT;
			ST7735_data(0x00);		// Normal: Top to Bottom; Left to Right; RGB
			break;
	}
}

void ST7735_AddrSet( uint16_t XS, uint16_t YS, uint16_t XE, uint16_t YE ) {
	ST7735_cmd( ST7735_CASET );		// Column address set
	ST7735_data(XS >> 8);
	ST7735_data(XS);
	ST7735_data(XE >> 8);
	ST7735_data(XE);

	ST7735_cmd( ST7735_RASET );		// Row address set
	ST7735_data(YS >> 8);
	ST7735_data(YS);
	ST7735_data(YE >> 8);
	ST7735_data(YE);

	ST7735_cmd(ST7735_RAMWR);		// Memory write
}

void ST7735_Clear( uint16_t color ) {
	uint16_t i;
	uint8_t  CH,CL;

	CH = color >> 8;
	CL = (uint8_t)color;

	ST7735_AddrSet( 0, 0, scr_width - 1, scr_height - 1 );
	for (i = 0; i < scr_width * scr_height; i++) {
		ST7735_data(CH);
		ST7735_data(CL);
	}
}

void ST7735_Pixel( uint16_t X, uint16_t Y, uint16_t color ) {
    ST7735_AddrSet( X, Y, X, Y );
    ST7735_data( color >> 8 );
    ST7735_data( (uint8_t)color );
}

void ST7735_HLine( uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color ) {
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

    ST7735_AddrSet( X1, Y, X2, Y );
	for ( uint16_t i = 0; i <= (X2 - X1); i++ ) {
		ST7735_data(CH);
		ST7735_data(CL);
	}
}

void ST7735_VLine( uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color ) {
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

    ST7735_AddrSet( X, Y1, X, Y2 );

	for ( uint16_t i = 0; i <= (Y2 - Y1); i++ ) {
		ST7735_data(CH);
		ST7735_data(CL);
	}
}

void ST7735_Line( int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint16_t color ) {
	int16_t dX 		= X2 - X1;
	int16_t dY 		= Y2 - Y1;
	int16_t dXsym 	= (dX > 0) ? 1 : -1;
	int16_t dYsym 	= (dY > 0) ? 1 : -1;

	if (dX == 0) {
		if (Y2>Y1)	ST7735_VLine( X1, Y1, Y2, color );
		else		ST7735_VLine(X1,Y2,Y1,color);
		return;
	}
	if (dY == 0) {
		if (X2>X1)	ST7735_HLine( X1, X2, Y1, color);
		else		ST7735_HLine(X2,X1,Y1,color);
		return;
	}

	dX *= dXsym;
	dY *= dYsym;
	int16_t dX2 = dX << 1;
	int16_t dY2 = dY << 1;
	int16_t di;

	if (dX >= dY) {
		di = dY2 - dX;
		while (X1 != X2) {
			ST7735_Pixel( X1, Y1, color );
			X1 += dXsym;
			if (di < 0) {
				di += dY2;
			} else {
				di += dY2 - dX2;
				Y1 += dYsym;
			}
		}
	} else {
		di = dX2 - dY;
		while (Y1 != Y2) {
			ST7735_Pixel( X1, Y1, color );
			Y1 += dYsym;
			if (di < 0) {
				di += dX2;
			} else {
				di += dX2 - dY2;
				X1 += dXsym;
			}
		}
	}
	ST7735_Pixel( X1, Y1, color );
}

void ST7735_Rect( uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color ) {
	ST7735_HLine( X1, X2, Y1, color );
	ST7735_HLine( X1, X2, Y2, color );
	ST7735_VLine( X1, Y1, Y2, color );
	ST7735_VLine( X2, Y1, Y2, color );
}

void ST7735_FillRect( uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color ) {
	uint16_t FS = (X2 - X1 + 1) * (Y2 - Y1 + 1);
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

	ST7735_AddrSet( X1, Y1, X2, Y2 );
	for ( uint16_t i = 0; i < FS; i++ ) {
		ST7735_data(CH);
		ST7735_data(CL);
	}
}

void ST7735_PutChar5x7( uint16_t X, uint16_t Y, uint8_t chr, uint16_t color ) {
	uint8_t buffer[5];
    uint8_t CH = color >> 8;
    uint8_t CL = (uint8_t)color;

//	memcpy( (uint8_t *)buffer, (uint8_t *)&Font5x7[(chr - 32) * 5], 5 );

	ST7735_AddrSet( X, Y, X + 4, Y + 6 );
	for ( uint16_t j = 0; j < 7; j++) {
        for ( uint16_t i = 0; i < 5; i++ ) {
    		if ( (buffer[i] >> j) & 0x01 ) {
    			ST7735_data(CH);
    			ST7735_data(CL);
    		} else {
    			ST7735_data(0x00);
    			ST7735_data(0x00);
    		}
    	}
    }
}

void ST7735_PutStr5x7( uint8_t X, uint8_t Y, char *str, uint16_t color ) {
    while ( *str ) {
        ST7735_PutChar5x7( X, Y, *str++, color );
        if (X < scr_width - 6) {
        	X += 6;
        }
        else if (Y < scr_height - 8) {
        	X = 0; Y += 8;
        }
        else {
        	X = 0; Y = 0;
        }
    }
}
