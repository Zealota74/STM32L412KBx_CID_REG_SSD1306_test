/*
 * sw_st7735.c
 *
 *  Created on: 21 sie 2021
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "../../SW_BOARD/sw_gpio.h"
#include "../../SW_TIMERS/sw_soft_timers.h"

#include "sw_st7735.h"



inline void CS_SET(void){

}

inline void CS_CLR(void){

}


inline void DC_SET(void){

}

inline void DC_CLR(void){

}

//==============================

void write_command(unsigned char cmd){
	DC_CLR();
	CS_CLR();
//	spiTx(cmd);
	CS_SET();
}

void write_data(unsigned char data){
	DC_SET();
	CS_CLR();
//	spiTx(data);
	CS_SET();
}

void lcd_clr(void){
	uint32_t i;

	write_command(ST7735_CASET);
	write_data(0);
	write_data(26);
	write_data(0);
	write_data(26+79);
	write_command(ST7735_RASET);
	write_data(0);
	write_data(1);
	write_data(0);
	write_data(1+159);
	write_command(ST7735_RAMWR);

	DC_SET();
	CS_CLR();

//	SPI1->CR2 = (11<<8);	// 12bit spi

	for(i=0; i<80*160; i++){
		while((SPI1->SR & 0x1800)==0x1800);
		SPI1->DR = 0x000;
	}
	CS_SET();

//	SPI1->CR2 = (7<<8);	// 8bit spi
}

void lcd_init(void){
//	spiInit();

	write_command(ST7735_SWRESET);
	delay_ms(10);
	write_command(ST7735_SLPOUT);
	delay_ms(10);
	write_command(ST7735_INVON);
	//write_command(ST7735_INVOFF);
	write_command(ST7735_MADCTL);
	//write_data(0x40);
	write_data(0x80);
	//write_data(0x88);
	write_command(ST7735_COLMOD);
	write_data(0x03);
	write_command(ST7735_NORON);

	lcd_clr();

	write_command(ST7735_DISPON);
}

