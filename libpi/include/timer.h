#ifndef _TIMER_H_
#define _TIMER_H_

//#include "rpi.h"

#define TIMER_BASE	(PERIPHERAL_BASE+0x3000)
#define TIMER_LO	(TIMER_BASE+0x04)
#define TIMER_HI	(TIMER_BASE+0x08)

void delay_usec(uint64_t usec);
void delay_msec(uint64_t msec);
void delay_sec(uint64_t sec);

uint64_t timer_get_usec(void);
uint64_t timer_get_usec_raw(void);

#endif
