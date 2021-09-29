/*
 * fonts.h
 *
 *  Created on: 26 paź 2015
 *      Author: simon
 */

#ifndef LIBS_FONTS_PL_H_
#define LIBS_FONTS_PL_H_

#include <stddef.h>
#include <stdint.h>

//#define UNICODE			1
#define UTF_8			2
//#define ISO8859_2		3
//#define WIN1250		4
//#define CP852			5

#define ASCII				19
#define UTF8_LOW			20

#define PL_CHARACTERS_COUNT	18

#ifdef UNICODE
#define		SPACE (L' ')
typedef wchar_t CHAR;
extern const wchar_t	utf_8Code[18];
#else
#define		SPACE (' ')
typedef char CHAR;

extern const wchar_t	uniCode[18];
extern const char		iso8859_2Code[18];
extern const char		win1250Code[18];
extern const char		cp852Code[18];
#endif


extern uint8_t polish_letters_UTF8( CHAR *codeChar );

#endif /* LIBS_FONTS_PL_H_ */
