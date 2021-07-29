/*
 * fonts.h
 *
 *  Created on: 26 paź 2015
 *      Author: simon
 */

#ifndef LIBS_FONTS_PL_H_
#define LIBS_FONTS_PL_H_

#define UNICODE		0
//#define UTF_8		1
//#define ISO8859_2	2
//#define WIN1250		3
//#define CP852		4

#define ASCII				19
#define PL_CHARACTERS_COUNT	18

extern const wchar_t	utf_8Code[18];
extern const wchar_t	uniCode[18];
extern const char		iso8859_2Code[18];
extern const char		win1250Code[18];
extern const char		cp852Code[18];

extern uint8_t polish_letters(uint8_t codePage, wchar_t codeChar);

#endif /* LIBS_FONTS_PL_H_ */
