#ifndef _TIMER_H_
#define _TIMER_H_

//#include "rpi.h"

void delay_usec( uint64_t usec );
void delay_msec( uint64_t msec );
void delay_sec( uint64_t sec );

uint64_t timer_get_usec( void );
uint64_t timer_get_usec_raw( void );

#endif
