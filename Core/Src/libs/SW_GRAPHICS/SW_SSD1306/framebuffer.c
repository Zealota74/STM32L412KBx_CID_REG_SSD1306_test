/*
 * framebuffer.c
 *
 *  Created on: 20.08.2018
 *      Author: simon
 */

#include "stdint.h"

#include "../../SW_STM_PERIPHS/sw_dma.h"
#include "framebuffer.h"

static 	 T_DISPLAY DispBuffer1[ FRAMEBUFFER_ALL ];
volatile T_DISPLAY * SpectrumBuffer = (T_DISPLAY *)DispBuffer1;

static 	 T_DISPLAY DispBuffer2_1[ FRAMEBUFFER_ALL ];
static 	 T_DISPLAY DispBuffer2_2[ FRAMEBUFFER_ALL ];
volatile T_DISPLAY * TextBufferProgram 	= (T_DISPLAY *)DispBuffer2_1;
volatile T_DISPLAY * TextBufferDisplay 	= (T_DISPLAY *)DispBuffer2_2;

volatile T_DISPLAY * TextBuffer 		= (T_DISPLAY *)DispBuffer2_1;

T_DISPLAY * Displays[] = { DispBuffer1, DispBuffer2_1, DispBuffer2_2 };

volatile uint8_t sync_flag1 = 0, sync_flag2 = 0;
volatile uint8_t refreshScreen = 0;


/*********************************** Switch buffer  **********************************/
 void sw_update_screen( void ) {

	if ( TextBuffer == TextBufferProgram )
		TextBuffer = TextBufferDisplay;
	else
		TextBuffer = TextBufferProgram;

	sync_flag2 = 2;
//	while( sync_flag2 ){}
	refreshScreen = 1;

//	clear_buffer_RAM( TextBuffer );
}
/***************************************************************************************/


void clear_buffer_RAM( volatile T_DISPLAY * buffer ) {
	sw_dma_memset_start( (uint8_t *)buffer, 0x00, FRAMEBUFFER_ALL );
}
