/*
 * menu.h
 *
 *  Created on: 10.10.2017
 *      Author: simon
 */

#ifndef LIBS_SW_GRAPHICS_MENU_H_
#define LIBS_SW_GRAPHICS_MENU_H_


#define SHOW_TIME		0
#define SET_DATE_TIME	1


enum {
	_fallTimeSet, _riseTimeSet, _peakSet, _peakStop, _brightSet, _gainSet, _sets, _themes
};
enum {
	_setHour, _setMin, _setSec
};


/*********************** Struktura definiująca zachowanie spektrum ************************************/
typedef struct {
	int32_t		fallTimeSet;
	int32_t		riseTimeSet;
	int32_t		peakTimeSet;
	uint16_t	peakStop;

	int8_t		brightSet;
	int8_t		gainSet;
	int8_t		sets;
	int8_t		colors;

	int32_t		fallAvgTimeSet;
	int32_t		riseAvgTimeSet;
	int32_t		peakAvgTimeSet;
} T_SETTINGS;
/******************************************************************************************************/


T_SETTINGS * struct_pointer_ret( void );

void plus_minus(void);
void switch_setting_modes( void );

void SW_SHOW_MENU_EVENT( void );
void SW_HIDE_MENU_EVENT( void );
void register_show_menu_event_callback( void (*callback)( void ) );
void register_hide_menu_event_callback( void (*callback)( void ) );

void sw_show_menu( void );
void sw_hide_menu( void );

void ir_decode_keys(void) ;

void set_menu_texts (void );
extern void show_clock( uint8_t *clearNumber, uint8_t mm );

void sw_menu_init(void);

#endif /* LIBS_SW_GRAPHICS_MENU_H_ */
