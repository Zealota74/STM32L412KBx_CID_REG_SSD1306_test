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
typedef struct {
	const uint8_t widthBits;			// width, in bits (or pixels), of the character
	const uint16_t offset;				// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
} FONT_CHAR_INFO;

// DEFINICJA
// Describes a single font
typedef struct {
	uint8_t heightPixels;				// height, in pages (8 pixels), of the font's characters
	wchar_t startChar;					// the first character in the font (e.g. in charInfo and data)
	uint8_t interspacePixels;			// number of pixels of interspace between characters
	uint8_t spacePixels;				// number of pixels of space character
	const FONT_CHAR_INFO* charInfo;		// pointer to array of char information
	const uint8_t 		* data;			// pointer to generated array of character visual representation
//    char 				* FontFileName;	// (Pointer) Font filename saved on SD card or 0 (null) otherwise
	uint16_t sizeArray;					// the size of array
} FONT_INFO;

extern const FONT_INFO MagicMatrix_3x7_FontInfo;
extern const FONT_INFO MagicMatrix_3x5_FontInfo;

extern const FONT_INFO MicrosoftSansSerif8ptFontInfo_var;
extern const FONT_INFO Calibri22ptFontInfo_var;

extern const FONT_INFO Arial6ptFontInfo_var;
extern const FONT_INFO LCD5x8FontInfo;
extern const FONT_INFO PMingLiU_ExtB20pt_bolditalic_fixedFontInfo;
extern const FONT_INFO PMingLiU_ExtB20pt_bolditalic_FontInfo;
extern const FONT_INFO ArialBlack_Digits15x20FontInfo;

extern const FONT_INFO ArialBlack_Digits21x27FontInfo;
extern const FONT_INFO Fixedsys_Digits18x27FontInfo;
extern const FONT_INFO Impact17x27FontInfo;
extern const FONT_INFO Terminal11x27FontInfo;
extern const FONT_INFO Unispace18x27FontInfo;
extern const FONT_INFO SimonDigits17x27FontInfo;
extern const FONT_INFO SimonDigits10x17FontInfo;
extern const FONT_INFO SimonDigits8x15FontInfo;
extern const FONT_INFO SimonDigits8x15FontInfo_v2;
extern const FONT_INFO SimonDigits8x15FontInfo_v3;
extern const FONT_INFO TerminalDigits8x18FontInfo;


#endif /* LIBS_FONTS_FONTS_COMMON_H_ */
