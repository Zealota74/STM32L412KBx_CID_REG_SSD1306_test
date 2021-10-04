/*
 * sw_itoa.h
 *
 *  Created on: 7 lip 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_SW_ITOA_H_
#define SRC_LIBS_SW_ITOA_H_

//extern char* itoa( int value, char *string, int radix ) ;
//extern char* utoa( unsigned long value, char *string, int radix ) ;

extern char* ltoa( long value, char *string, int radix ) ;
extern char* ultoa( unsigned long value, char *string, int radix ) ;
extern char* strrev( char *str );

#endif /* SRC_LIBS_SW_ITOA_H_ */
