/*
 * ms_sanserrif.h
 *
 *  Created on: 13 lis 2015
 *      Author: simon
 */

#ifndef LIBS_FONTS_FONTS_COMMON_H_
#define LIBS_FONTS_FONTS_COMMON_H_

#include <stdlib.h>		// Potrzebne żeby mieć typ danych wchar_t
#include <stdint.h>

#define FONTx1			1
#define FONTx2			2
#define FONTx3			3
#define FONTx4			4
#define FONTx5			5

// ==========================================================================
// structure definition
// ==========================================================================

// This structure describes a single character's display information
typedef struct
{
	const uint8_t widthBits;				// width, in bits (or pixels), of the character
	const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array

} FONT_CHAR_INFO;

// DEFINICJA
// Describes a single font
typedef struct
{
	uint8_t heightPixels;				// height, in pages (8 pixels), of the font's characters
	wchar_t startChar;					// the first character in the font (e.g. in charInfo and data)
	uint8_t interspacePixels;			// number of pixels of interspace between characters
	uint8_t spacePixels;				// number of pixels of space character
	const FONT_CHAR_INFO* charInfo;		// pointer to array of char information
	const char 			* data;			// pointer to generated array of character visual representation
    char * FontFileName;				// (Pointer) Font filename saved on SD card or 0 (null) otherwise
	uint16_t sizeArray;					// the size of array

} FONT_INFO;

extern const FONT_INFO Arial24ptFontInfo_fix;
extern const FONT_INFO LCD236ptFontInfo_fix;
extern const FONT_INFO Digital_7Mono36ptFontInfo_fix;
extern const FONT_INFO LEDCounter736ptFontInfo_fix;
extern const FONT_INFO DotMatrixFontInfo;
extern const FONT_INFO SimonFirst26x43FontInfo;
extern const FONT_INFO SubwayTicker30ptFontInfo;

extern const FONT_INFO MicrosoftSansSerif8ptFontInfo_var;
extern const FONT_INFO Arial32ptFontInfo_fix;


//extern const FONT_INFO DotMatrixFontInfo;
//extern const FONT_INFO SevenSegmentFontInfo;

#endif /* LIBS_FONTS_FONTS_COMMON_H_ */
