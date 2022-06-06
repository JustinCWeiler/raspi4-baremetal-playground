#include <stdint.h>
#include <stddef.h>

#define PERIPHERAL_BASE 0xFE000000

#include "gpio.c"

#define BLINK_WAIT 0xC0000

static void wait(volatile size_t w) {
	while (w--) ;
}

__attribute__((flatten, noreturn, nothrow, cold))
void main(void) {
	for (int i = 0; i < 2; i++) {
		gpio_act_on();
		wait(BLINK_WAIT);

		gpio_act_off();
		wait(BLINK_WAIT);
	}

	while (1) asm volatile ("wfe");
}
