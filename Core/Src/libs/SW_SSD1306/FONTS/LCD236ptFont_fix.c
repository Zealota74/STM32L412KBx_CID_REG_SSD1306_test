/*
 * LCD236ptFont.c
 *
 *  Created on: 18 gru 2016
 *      Author: simon
 */


/*
*  LCD236pt.c
*  Created on: 18.12.2016 00:53:37
*       Autor: Mirosław Kardaś
*       www.atnel.pl
*/
#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#include "../fonts_common.h"

//  Font data for LCD2 36pt

// Character bitmaps for LCD2 36pt
const char LCD236ptBitmaps[] PROGMEM =
{
	    // @0   [0]   (26 px wide)
	     0x00, 0x7F, 0xFC, 0x00,  //   .........#############....    //
	     0x00, 0xFF, 0xFE, 0x00,  //   ........###############...    //
	     0x03, 0xFF, 0xFF, 0x00,  //   ......##################..    //
	     0x07, 0xFF, 0xFF, 0x80,  //   .....####################.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x1E, 0x00, 0x07, 0x80,  //   ...####..............####.    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1C, 0x00, 0x0E, 0x00,  //   ...###..............###...    //
	     0x08, 0x00, 0x04, 0x00,  //   ....#................#....    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x18, 0x00, 0x0C, 0x00,  //   ...##...............##....    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x78, 0x00, 0x1E, 0x00,  //   .####..............####...    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0xF0, 0x00, 0x3C, 0x00,  //   ####..............####....    //
	     0xF0, 0x00, 0x78, 0x00,  //   ####.............####.....    //
	     0xF0, 0x00, 0x78, 0x00,  //   ####.............####.....    //
	     0xF0, 0x00, 0x78, 0x00,  //   ####.............####.....    //
	     0x6F, 0xFF, 0xB0, 0x00,  //   .##.#############.##......    //
	     0x5F, 0xFF, 0xC0, 0x00,  //   .#.###############........    //
	     0x3F, 0xFF, 0xC0, 0x00,  //   ..################........    //
	     0x1F, 0xFF, 0x80, 0x00,  //   ...##############.........    //

	     // @152   [1]   (26 px wide)
	     0x00, 0x01, 0x00, 0x00,  //   ...............#..........    //
	     0x00, 0x03, 0x80, 0x00,  //   ..............###.........    //
	     0x00, 0x07, 0x80, 0x00,  //   .............####.........    //
	     0x00, 0x0F, 0x80, 0x00,  //   ............#####.........    //
	     0x00, 0x3F, 0x80, 0x00,  //   ..........#######.........    //
	     0x00, 0x77, 0x80, 0x00,  //   .........###.####.........    //
	     0x00, 0xFF, 0x80, 0x00,  //   ........#########.........    //
	     0x00, 0xFF, 0x80, 0x00,  //   ........#########.........    //
	     0x00, 0xFF, 0x80, 0x00,  //   ........#########.........    //
	     0x00, 0x6F, 0x00, 0x00,  //   .........##.####..........    //
	     0x00, 0x0F, 0x00, 0x00,  //   ............####..........    //
	     0x00, 0x0F, 0x00, 0x00,  //   ............####..........    //
	     0x00, 0x0F, 0x00, 0x00,  //   ............####..........    //
	     0x00, 0x1F, 0x00, 0x00,  //   ...........#####..........    //
	     0x00, 0x1F, 0x00, 0x00,  //   ...........#####..........    //
	     0x00, 0x1F, 0x00, 0x00,  //   ...........#####..........    //
	     0x00, 0x0E, 0x00, 0x00,  //   ............###...........    //
	     0x00, 0x04, 0x00, 0x00,  //   .............#............    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x0C, 0x00, 0x00,  //   ............##............    //
	     0x00, 0x1E, 0x00, 0x00,  //   ...........####...........    //
	     0x00, 0x3E, 0x00, 0x00,  //   ..........#####...........    //
	     0x00, 0x3E, 0x00, 0x00,  //   ..........#####...........    //
	     0x00, 0x3C, 0x00, 0x00,  //   ..........####............    //
	     0x00, 0x3C, 0x00, 0x00,  //   ..........####............    //
	     0x00, 0x3C, 0x00, 0x00,  //   ..........####............    //
	     0x00, 0x7C, 0x00, 0x00,  //   .........#####............    //
	     0x00, 0x7C, 0x00, 0x00,  //   .........#####............    //
	     0x00, 0x7C, 0x00, 0x00,  //   .........#####............    //
	     0x00, 0x7C, 0x00, 0x00,  //   .........#####............    //
	     0x00, 0x78, 0x00, 0x00,  //   .........####.............    //
	     0x00, 0x78, 0x00, 0x00,  //   .........####.............    //
	     0x00, 0x78, 0x00, 0x00,  //   .........####.............    //
	     0x00, 0xF8, 0x00, 0x00,  //   ........#####.............    //
	     0x00, 0xF8, 0x00, 0x00,  //   ........#####.............    //
	     0x00, 0x70, 0x00, 0x00,  //   .........###..............    //
	     0x00, 0x20, 0x00, 0x00,  //   ..........#...............    //

	     // @304   [2]   (26 px wide)
	     0x00, 0x7F, 0xFE, 0x00,  //   .........##############...    //
	     0x00, 0xFF, 0xFE, 0x00,  //   ........###############...    //
	     0x02, 0xFF, 0xFF, 0x00,  //   ......#.################..    //
	     0x07, 0x7F, 0xFD, 0x80,  //   .....###.#############.##.    //
	     0x07, 0x80, 0x03, 0xC0,  //   .....####.............####    //
	     0x0F, 0x00, 0x03, 0xC0,  //   ....####..............####    //
	     0x07, 0x00, 0x07, 0x80,  //   .....###.............####.    //
	     0x00, 0x00, 0x07, 0x80,  //   .....................####.    //
	     0x00, 0x00, 0x07, 0x80,  //   .....................####.    //
	     0x00, 0x00, 0x07, 0x80,  //   .....................####.    //
	     0x00, 0x00, 0x07, 0x80,  //   .....................####.    //
	     0x00, 0x00, 0x07, 0x80,  //   .....................####.    //
	     0x00, 0x00, 0x07, 0x80,  //   .....................####.    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x07, 0x00,  //   .....................###..    //
	     0x01, 0xFF, 0xF6, 0x00,  //   .......#############.##...    //
	     0x03, 0xFF, 0xF8, 0x00,  //   ......###############.....    //
	     0x0F, 0xFF, 0xF8, 0x00,  //   ....#################.....    //
	     0x1B, 0xFF, 0xF0, 0x00,  //   ...##.##############......    //
	     0x3C, 0x00, 0x00, 0x00,  //   ..####....................    //
	     0x3C, 0x00, 0x00, 0x00,  //   ..####....................    //
	     0x3C, 0x00, 0x00, 0x00,  //   ..####....................    //
	     0x3C, 0x00, 0x00, 0x00,  //   ..####....................    //
	     0x3C, 0x00, 0x00, 0x00,  //   ..####....................    //
	     0x3C, 0x00, 0x00, 0x00,  //   ..####....................    //
	     0x78, 0x00, 0x00, 0x00,  //   .####.....................    //
	     0x78, 0x00, 0x00, 0x00,  //   .####.....................    //
	     0x78, 0x00, 0x00, 0x00,  //   .####.....................    //
	     0x78, 0x00, 0x00, 0x00,  //   .####.....................    //
	     0x78, 0x00, 0x00, 0x00,  //   .####.....................    //
	     0x78, 0x00, 0x00, 0x00,  //   .####.....................    //
	     0x70, 0x00, 0x00, 0x00,  //   .###......................    //
	     0xEF, 0xFF, 0xC0, 0x00,  //   ###.##############........    //
	     0xDF, 0xFF, 0xC0, 0x00,  //   ##.###############........    //
	     0xBF, 0xFF, 0xE0, 0x00,  //   #.#################.......    //
	     0x7F, 0xFF, 0xF0, 0x00,  //   .###################......    //

	     // @456   [3]   (26 px wide)
	     0x03, 0xFF, 0xF8, 0x00,  //   ......###############.....    //
	     0x07, 0xFF, 0xFC, 0x00,  //   .....#################....    //
	     0x07, 0xFF, 0xFE, 0x00,  //   .....##################...    //
	     0x03, 0xFF, 0xFF, 0x00,  //   ......##################..    //
	     0x00, 0x00, 0x07, 0x80,  //   .....................####.    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x0F, 0x00,  //   ....................####..    //
	     0x00, 0x00, 0x1E, 0x00,  //   ...................####...    //
	     0x00, 0x00, 0x1E, 0x00,  //   ...................####...    //
	     0x00, 0x00, 0x1E, 0x00,  //   ...................####...    //
	     0x00, 0x00, 0x1E, 0x00,  //   ...................####...    //
	     0x00, 0x00, 0x1E, 0x00,  //   ...................####...    //
	     0x01, 0xFF, 0xE8, 0x00,  //   .......############.#.....    //
	     0x03, 0xFF, 0xF0, 0x00,  //   ......##############......    //
	     0x03, 0xFF, 0xF0, 0x00,  //   ......##############......    //
	     0x03, 0xFF, 0xF8, 0x00,  //   ......###############.....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x7C, 0x00,  //   .................#####....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x3F, 0xFF, 0x70, 0x00,  //   ..##############.###......    //
	     0x7F, 0xFF, 0xC0, 0x00,  //   .#################........    //
	     0xFF, 0xFF, 0x80, 0x00,  //   #################.........    //
	     0x7F, 0xFF, 0x00, 0x00,  //   .###############..........    //

	     // @608   [4]   (26 px wide)
	     0x00, 0x00, 0x02, 0x00,  //   ......................#...    //
	     0x00, 0x00, 0x07, 0x00,  //   .....................###..    //
	     0x0C, 0x00, 0x0F, 0x00,  //   ....##..............####..    //
	     0x1C, 0x00, 0x0F, 0x00,  //   ...###..............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x3C, 0x00, 0x1F, 0x00,  //   ..####.............#####..    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x78, 0x00, 0x1E, 0x00,  //   .####..............####...    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x70, 0x00, 0x1C, 0x00,  //   .###...............###....    //
	     0x27, 0xFF, 0xD8, 0x00,  //   ..#..#############.##.....    //
	     0x0F, 0xFF, 0xE0, 0x00,  //   ....###############.......    //
	     0x1F, 0xFF, 0xE0, 0x00,  //   ...################.......    //
	     0x0F, 0xFF, 0xD8, 0x00,  //   ....##############.##.....    //
	     0x00, 0x00, 0x38, 0x00,  //   ..................###.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x01, 0xE0, 0x00,  //   ...............####.......    //
	     0x00, 0x00, 0xE0, 0x00,  //   ................###.......    //
	     0x00, 0x00, 0x40, 0x00,  //   .................#........    //

	     // @760   [5]   (26 px wide)
	     0x03, 0xFF, 0xFF, 0x80,  //   ......###################.    //
	     0x05, 0xFF, 0xFF, 0x00,  //   .....#.#################..    //
	     0x0F, 0xFF, 0xFE, 0x00,  //   ....###################...    //
	     0x0E, 0xFF, 0xFC, 0x00,  //   ....###.##############....    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x1E, 0x00, 0x00, 0x00,  //   ...####...................    //
	     0x1E, 0x00, 0x00, 0x00,  //   ...####...................    //
	     0x1E, 0x00, 0x00, 0x00,  //   ...####...................    //
	     0x1E, 0x00, 0x00, 0x00,  //   ...####...................    //
	     0x1E, 0x00, 0x00, 0x00,  //   ...####...................    //
	     0x1E, 0x00, 0x00, 0x00,  //   ...####...................    //
	     0x1E, 0x00, 0x00, 0x00,  //   ...####...................    //
	     0x3C, 0x00, 0x00, 0x00,  //   ..####....................    //
	     0x3B, 0xFF, 0xF0, 0x00,  //   ..###.##############......    //
	     0x07, 0xFF, 0xF0, 0x00,  //   .....###############......    //
	     0x07, 0xFF, 0xF0, 0x00,  //   .....###############......    //
	     0x03, 0xFF, 0xEC, 0x00,  //   ......#############.##....    //
	     0x00, 0x00, 0x1E, 0x00,  //   ...................####...    //
	     0x00, 0x00, 0x1E, 0x00,  //   ...................####...    //
	     0x00, 0x00, 0x1E, 0x00,  //   ...................####...    //
	     0x00, 0x00, 0x1E, 0x00,  //   ...................####...    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x40, 0x00, 0x3C, 0x00,  //   .#................####....    //
	     0xE0, 0x00, 0x3C, 0x00,  //   ###...............####....    //
	     0xF0, 0x00, 0x78, 0x00,  //   ####.............####.....    //
	     0xF0, 0x00, 0x78, 0x00,  //   ####.............####.....    //
	     0x6F, 0xFF, 0xB0, 0x00,  //   .##.#############.##......    //
	     0x5F, 0xFF, 0xC0, 0x00,  //   .#.###############........    //
	     0x3F, 0xFF, 0xC0, 0x00,  //   ..################........    //
	     0x1F, 0xFF, 0x80, 0x00,  //   ...##############.........    //

	     // @912   [6]   (26 px wide)
	     0x00, 0x3F, 0xFE, 0x00,  //   ..........#############...    //
	     0x00, 0x7F, 0xFF, 0x00,  //   .........###############..    //
	     0x01, 0xFF, 0xFF, 0x00,  //   .......#################..    //
	     0x03, 0xFF, 0xFE, 0x00,  //   ......#################...    //
	     0x07, 0x80, 0x00, 0x00,  //   .....####.................    //
	     0x07, 0x80, 0x00, 0x00,  //   .....####.................    //
	     0x07, 0x80, 0x00, 0x00,  //   .....####.................    //
	     0x07, 0x80, 0x00, 0x00,  //   .....####.................    //
	     0x07, 0x80, 0x00, 0x00,  //   .....####.................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x0F, 0x00, 0x00, 0x00,  //   ....####..................    //
	     0x1E, 0x00, 0x00, 0x00,  //   ...####...................    //
	     0x1D, 0xFF, 0xF8, 0x00,  //   ...###.##############.....    //
	     0x03, 0xFF, 0xF8, 0x00,  //   ......###############.....    //
	     0x1B, 0xFF, 0xF8, 0x00,  //   ...##.###############.....    //
	     0x1D, 0xFF, 0xF6, 0x00,  //   ...###.#############.##...    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x3C, 0x00, 0x0F, 0x00,  //   ..####..............####..    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x78, 0x00, 0x1E, 0x00,  //   .####..............####...    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x37, 0xFF, 0xD8, 0x00,  //   ..##.#############.##.....    //
	     0x2F, 0xFF, 0xE0, 0x00,  //   ..#.###############.......    //
	     0x1F, 0xFF, 0xE0, 0x00,  //   ...################.......    //
	     0x0F, 0xFF, 0xC0, 0x00,  //   ....##############........    //

	     // @1064   [7]   (26 px wide)
	     0x03, 0xFF, 0xFE, 0x00,  //   ......#################...    //
	     0x07, 0xFF, 0xFF, 0x00,  //   .....###################..    //
	     0x07, 0xFF, 0xFF, 0x00,  //   .....###################..    //
	     0x03, 0xFF, 0xFE, 0x00,  //   ......#################...    //
	     0x00, 0x00, 0x04, 0x00,  //   .....................#....    //
	     0x00, 0x00, 0x0C, 0x00,  //   ....................##....    //
	     0x00, 0x00, 0x1C, 0x00,  //   ...................###....    //
	     0x00, 0x00, 0x1C, 0x00,  //   ...................###....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0xF8, 0x00,  //   ................#####.....    //
	     0x00, 0x01, 0xF0, 0x00,  //   ...............#####......    //
	     0x00, 0x01, 0xF0, 0x00,  //   ...............#####......    //
	     0x00, 0x01, 0xE0, 0x00,  //   ...............####.......    //
	     0x00, 0x01, 0xC0, 0x00,  //   ...............###........    //
	     0x00, 0x01, 0xC0, 0x00,  //   ...............###........    //
	     0x00, 0x01, 0x80, 0x00,  //   ...............##.........    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x18, 0x00, 0x00,  //   ...........##.............    //
	     0x00, 0x38, 0x00, 0x00,  //   ..........###.............    //
	     0x00, 0x78, 0x00, 0x00,  //   .........####.............    //
	     0x00, 0x78, 0x00, 0x00,  //   .........####.............    //
	     0x00, 0xF8, 0x00, 0x00,  //   ........#####.............    //
	     0x01, 0xF0, 0x00, 0x00,  //   .......#####..............    //
	     0x01, 0xF0, 0x00, 0x00,  //   .......#####..............    //
	     0x03, 0xE0, 0x00, 0x00,  //   ......#####...............    //
	     0x07, 0xC0, 0x00, 0x00,  //   .....#####................    //
	     0x07, 0xC0, 0x00, 0x00,  //   .....#####................    //
	     0x0F, 0x80, 0x00, 0x00,  //   ....#####.................    //
	     0x1F, 0x00, 0x00, 0x00,  //   ...#####..................    //
	     0x1F, 0x00, 0x00, 0x00,  //   ...#####..................    //
	     0x3E, 0x00, 0x00, 0x00,  //   ..#####...................    //
	     0x3C, 0x00, 0x00, 0x00,  //   ..####....................    //
	     0x7C, 0x00, 0x00, 0x00,  //   .#####....................    //
	     0x78, 0x00, 0x00, 0x00,  //   .####.....................    //
	     0x70, 0x00, 0x00, 0x00,  //   .###......................    //
	     0x70, 0x00, 0x00, 0x00,  //   .###......................    //

	     // @1216   [8]   (26 px wide)
	     0x00, 0x7F, 0xFC, 0x00,  //   .........#############....    //
	     0x00, 0xFF, 0xFE, 0x00,  //   ........###############...    //
	     0x03, 0xFF, 0xFF, 0x00,  //   ......##################..    //
	     0x07, 0xFF, 0xFF, 0x80,  //   .....####################.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x0F, 0x00, 0x07, 0x80,  //   ....####.............####.    //
	     0x1E, 0x00, 0x07, 0x80,  //   ...####..............####.    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1C, 0x00, 0x0E, 0x00,  //   ...###..............###...    //
	     0x0B, 0xFF, 0xF4, 0x00,  //   ....#.##############.#....    //
	     0x07, 0xFF, 0xF0, 0x00,  //   .....###############......    //
	     0x07, 0xFF, 0xF0, 0x00,  //   .....###############......    //
	     0x1B, 0xFF, 0xEC, 0x00,  //   ...##.#############.##....    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x78, 0x00, 0x1E, 0x00,  //   .####..............####...    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0x78, 0x00, 0x3C, 0x00,  //   .####.............####....    //
	     0xF0, 0x00, 0x3C, 0x00,  //   ####..............####....    //
	     0xF0, 0x00, 0x78, 0x00,  //   ####.............####.....    //
	     0xF0, 0x00, 0x78, 0x00,  //   ####.............####.....    //
	     0xF0, 0x00, 0x78, 0x00,  //   ####.............####.....    //
	     0x6F, 0xFF, 0xB0, 0x00,  //   .##.#############.##......    //
	     0x5F, 0xFF, 0xC0, 0x00,  //   .#.###############........    //
	     0x3F, 0xFF, 0xC0, 0x00,  //   ..################........    //
	     0x1F, 0xFF, 0x80, 0x00,  //   ...##############.........    //

	     // @1368   [9]   (26 px wide)
	     0x01, 0xFF, 0xF8, 0x00,  //   .......##############.....    //
	     0x03, 0xFF, 0xFC, 0x00,  //   ......################....    //
	     0x07, 0xFF, 0xFE, 0x00,  //   .....##################...    //
	     0x0D, 0xFF, 0xFF, 0x00,  //   ....##.#################..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x1E, 0x00, 0x0F, 0x00,  //   ...####.............####..    //
	     0x3C, 0x00, 0x0F, 0x00,  //   ..####..............####..    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x3C, 0x00, 0x1E, 0x00,  //   ..####.............####...    //
	     0x78, 0x00, 0x1E, 0x00,  //   .####..............####...    //
	     0x78, 0x00, 0x1C, 0x00,  //   .####..............###....    //
	     0x37, 0xFF, 0xEC, 0x00,  //   ..##.##############.##....    //
	     0x0F, 0xFF, 0xE0, 0x00,  //   ....###############.......    //
	     0x0F, 0xFF, 0xFC, 0x00,  //   ....##################....    //
	     0x07, 0xFF, 0xDC, 0x00,  //   .....#############.###....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x3C, 0x00,  //   ..................####....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0x78, 0x00,  //   .................####.....    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x00, 0x00, 0xF0, 0x00,  //   ................####......    //
	     0x1F, 0xFF, 0x60, 0x00,  //   ...#############.##.......    //
	     0x3F, 0xFF, 0x80, 0x00,  //   ..###############.........    //
	     0x7F, 0xFF, 0x80, 0x00,  //   .################.........    //
	     0x3F, 0xFF, 0x00, 0x00,  //   ..##############..........    //

	     // @1520   [:]   (26 px wide)
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x06, 0x00, 0x00,  //   .............##...........    //
	     0x00, 0x0F, 0x00, 0x00,  //   ............####..........    //
	     0x00, 0x0F, 0x00, 0x00,  //   ............####..........    //
	     0x00, 0x0E, 0x00, 0x00,  //   ............###...........    //
	     0x00, 0x04, 0x00, 0x00,  //   .............#............    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x18, 0x00, 0x00,  //   ...........##.............    //
	     0x00, 0x3C, 0x00, 0x00,  //   ..........####............    //
	     0x00, 0x3C, 0x00, 0x00,  //   ..........####............    //
	     0x00, 0x38, 0x00, 0x00,  //   ..........###.............    //
	     0x00, 0x10, 0x00, 0x00,  //   ...........#..............    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
	     0x00, 0x00, 0x00, 0x00,  //   ..........................    //
};

// Character descriptors for LCD2 36pt
// { [Char width in bits], [Offset into LCD236ptCharBitmaps in bytes] }
const FONT_CHAR_INFO LCD236ptDescriptors[] PROGMEM =
{
	    { 26,0 },		// 0  (48)
	    { 26,152 },		// 1  (49)
	    { 26,304 },		// 2  (50)
	    { 26,456 },		// 3  (51)
	    { 26,608 },		// 4  (52)
	    { 26,760 },		// 5  (53)
	    { 26,912 },		// 6  (54)
	    { 26,1064 },		// 7  (55)
	    { 26,1216 },		// 8  (56)
	    { 26,1368 },		// 9  (57)
	    { 26,1520 },		// :  (58)
};

// Font information for LCD2 36pt
const FONT_INFO LCD236ptFontInfo_fix PROGMEM =
{
    38,  // Character height
    L'0',  //  Start character
    2,  //  number of pixels of interspace between characters
    4,  // number of pixels of space character
    LCD236ptDescriptors, // Character descriptor array
    LCD236ptBitmaps, // Character bitmap array
    0, // (Pointer) Font filename saved on SD card or 0 (null) otherwise
	sizeof(LCD236ptDescriptors) / sizeof(LCD236ptDescriptors[0]),

};
