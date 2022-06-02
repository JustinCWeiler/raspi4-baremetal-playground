#include "rpi.h"

#include "timer.h"

void delay_usec(uint64_t usec) {
	uint64_t now = timer_get_usec_raw();
	while (timer_get_usec_raw() - now < usec) ;
	MB_RD;
}

void delay_msec(uint64_t msec) {
	delay_usec(1000*msec);
}

void delay_sec(uint64_t sec) {
	delay_msec(1000*sec);
}


uint64_t timer_get_usec(void) {
	uint64_t ret = timer_get_usec_raw();
	MB_RD;
	return ret;
}

uint64_t timer_get_usec_raw(void) {
	return ((uint64_t)GET32(TIMER_HI) << 32) | GET32(TIMER_LO);
}
