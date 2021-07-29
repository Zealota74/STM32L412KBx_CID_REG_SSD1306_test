/*
 * fonts.c
 *
 *  Created on: 26 paź 2015
 *      Author: simon
 */


#include "../../libs/SW_GRAPHICS/fonts_pl.h"



#ifdef UTF_8
const wchar_t utf_8Code[18] = {
	0xC59A,  /*Ś, */
	0xC5B9,  /*Ź, */
	0xC59B,  /*ś, */
	0xC5BA,  /*ź, */
	0xC581,  /*Ł, */
	0xC484,  /*Ą, */
	0xC5BB,  /*Ż, */
	0xC582,  /*ł, */
	0xC485,  /*ą, */
	0xC5BC,  /*ż, */
	0xC486,  /*Ć, */
	0xC498,  /*Ę, */
	0xC583,  /*Ń, */
	0xC393,  /*Ó, */
	0xC487,  /*ć, */
	0xC499,  /*ę, */
	0xC584,  /*ń, */
	0xC3B3,  /*ó, */
};
#endif

#ifdef UNICODE
const wchar_t uniCode[18] = {
	0x015A,  /*Ś, */
	0x0179,  /*Ź, */
	0x015B,  /*ś, */
	0x017A,  /*ź, */
	0x0141,  /*Ł, */
	0x0104,  /*Ą, */
	0x017B,  /*Ż, */
	0x0142,  /*ł, */
	0x0105,  /*ą, */
	0x017C,  /*ż, */
	0x0106,  /*Ć, */
	0x0118,  /*Ę, */
	0x0143,  /*Ń, */
	0x00D3,  /*Ó, */
	0x0107,  /*ć, */
	0x0119,  /*ę, */
	0x0144,  /*ń, */
	0x00F3,  /*ó, */
};
#endif

#ifdef WIN1250
const char win1250Code[] = {
	0x8C,  /*Ś, */
	0x8F,  /*Ź, */
	0x9C,  /*ś, */
	0x9F,  /*ź, */
	0xA3,  /*Ł, */
	0xA5,  /*Ą, */
	0xAF,  /*Ż, */
	0xB3,  /*ł, */
	0xB9,  /*ą, */
	0xBF,  /*ż, */
	0xC6,  /*Ć, */
	0xCA,  /*Ę, */
	0xD1,  /*Ń, */
	0xD3,  /*Ó, */
	0xE6,  /*ć, */
	0xEA,  /*ę, */
	0xF1,  /*ń, */
	0xF3,  /*ó, */
};
#endif

#ifdef ISO8859_2
const char iso8859_2Code[] = {
	0xA6,  /*Ś, */
	0xAC,  /*Ź, */
	0xB6,  /*ś, */
	0xBC,  /*ź, */
	0xA3,  /*Ł, */
	0xA1,  /*Ą, */
	0xAF,  /*Ż, */
	0xB3,  /*ł, */
	0xB1,  /*ą, */
	0xBF,  /*ż, */
	0xC6,  /*Ć, */
	0xCA,  /*Ę, */
	0xD1,  /*Ń, */
	0xD3,  /*Ó, */
	0xE6,  /*ć, */
	0xEA,  /*ę, */
	0xF1,  /*ń, */
	0xF3,  /*ó, */
};
#endif

#ifdef CP852
const char cp852Code[] = {
	0x97,  /*Ś, */
	0x8D,  /*Ź, */
	0x98,  /*ś, */
	0xAB,  /*ź, */
	0x9D,  /*Ł, */
	0xA4,  /*Ą, */
	0xBD,  /*Ż, */
	0x88,  /*ł, */
	0xA5,  /*ą, */
	0xBE,  /*ż, */
	0x8F,  /*Ć, */
	0xA8,  /*Ę, */
	0xE3,  /*Ń, */
	0xE0,  /*Ó, */
	0x86,  /*ć, */
	0xA9,  /*ę, */
	0xE4,  /*ń, */
	0xA2,  /*ó, */
};
#endif

/***** Zwraca numer znaku w tablicy ************************/
uint8_t polish_letters(uint8_t codePage, wchar_t codeChar) {

	uint8_t i;
	uint8_t tmp=ASCII;

	for (i=0; i<18; i++) {

#ifdef UNICODE
		if (codePage == UNICODE) {
			if ( codeChar == uniCode[i]) {
				return i;
			}
		}
#endif

#ifdef UTF_8
		if (codePage == UTF_8) {
			if ( codeChar == pgm_read_byte(utf_8Code+i) )
				return i;
		}
#endif

#ifdef ISO8859_2
		if (codePage == ISO8859_2) {
			if ( codeChar == pgm_read_byte(iso8859_2Code+i) )
				return i;
		}
#endif

#ifdef WIN1250
		if (codePage == WIN1250) {
			if ( codeChar == pgm_read_byte(win1250Code+i) )
				return i;
		}
#endif

#ifdef CP852
		if (codePage == CP852) {
			if ( codeChar == pgm_read_byte(cp852Code+i) )
				return i;
		}
#endif
	}
	return tmp;
}
/************************************************************/

