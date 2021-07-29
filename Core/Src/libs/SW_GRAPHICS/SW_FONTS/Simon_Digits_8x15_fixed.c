#include "../fonts_common.h"

//  Font data for PMingLiU-ExtB 20pt_bolditalic

// Character bitmaps for PMingLiU-ExtB 20pt_bolditalic
// Character bitmaps for ArialBlack_Digits15x20
const uint8_t SimonDigits8x15Bitmaps[] = {
	0x7E,0xBD,0xC3,0xC3,0xC3,0xC3,0x81,0x00,      // char 0  (48)
	0x81,0xC3,0xC3,0xC3,0xC3,0xBD,0x7E,
	0x00,0x01,0x03,0x03,0x03,0x03,0x01,0x00,      // char 1  (49)
	0x01,0x03,0x03,0x03,0x03,0x01,0x00,
	0x7E,0x3D,0x03,0x03,0x03,0x03,0x19,0x3C,      // char 2  (50)
	0x98,0xC0,0xC0,0xC0,0xC0,0xBC,0x7E,
	0x7E,0x3D,0x03,0x03,0x03,0x03,0x19,0x3C,      // char 3  (51)
	0x19,0x03,0x03,0x03,0x03,0x3D,0x7E,
	0x00,0x81,0xC3,0xC3,0xC3,0xC3,0x99,0x3C,      // char 4  (52)
	0x19,0x03,0x03,0x03,0x03,0x01,0x00,
	0x7E,0xBC,0xC0,0xC0,0xC0,0xC0,0x98,0x3C,      // char 5  (53)
	0x19,0x03,0x03,0x03,0x03,0x3D,0x7E,
	0x7E,0xBC,0xC0,0xC0,0xC0,0xC0,0x98,0x3C,      // char 6  (54)
	0x99,0xC3,0xC3,0xC3,0xC3,0xBD,0x7E,
	0x7E,0x3D,0x03,0x03,0x03,0x03,0x01,0x00,      // char 7  (55)
	0x01,0x03,0x03,0x03,0x03,0x01,0x00,
	0x7E,0xBD,0xC3,0xC3,0xC3,0xC3,0x99,0x3C,      // char 8  (56)
	0x99,0xC3,0xC3,0xC3,0xC3,0xBD,0x7E,
	0x7E,0xBD,0xC3,0xC3,0xC3,0xC3,0x99,0x3C,      // char 9  (57)
	0x19,0x03,0x03,0x03,0x03,0x3D,0x7E,
	0x00,0x00,0x00,0x40,0xC0,0xC0,0x40,0x00,      // char :  (58)
	0x40,0xC0,0xC0,0x40,0x00,0x00,0x00,
};

// descriptors
const FONT_CHAR_INFO SimonDigits8x15Descriptors[] = {
	{ 8,0 },	// 0  (48)
	{ 8,15 },	// 1  (49)
	{ 8,30 },	// 2  (50)
	{ 8,45 },	// 3  (51)
	{ 8,60 },	// 4  (52)
	{ 8,75 },	// 5  (53)
	{ 8,90 },	// 6  (54)
	{ 8,105 },	// 7  (55)
	{ 8,120 },	// 8  (56)
	{ 8,135 },	// 9  (57)
	{ 4,150 },	// :  (58)
};

// Font information
const FONT_INFO SimonDigits8x15FontInfo = {
	15,		// Character height
	'0',	// Start character
	1,		// number of pixels of interspace between characters
	2,		// number of pixels of space character
	SimonDigits8x15Descriptors,	// number of pixels of space character
	SimonDigits8x15Bitmaps,	// number of pixels of space character
	sizeof(SimonDigits8x15Descriptors) / sizeof(SimonDigits8x15Descriptors[0]),
};




