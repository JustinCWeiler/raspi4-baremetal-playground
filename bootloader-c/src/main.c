#include <stdint.h>
#include <stddef.h>

#define PERIPHERAL_BASE 0xFE000000

#define MB_RDWR asm volatile ("dsb sy")
#define MB_RD asm volatile ("dsb ld")
#define MB_WR asm volatile ("dsb st")

#include "gpio.c"
#include "uart.c"

#define BLINK_WAIT 0xC0000

static void wait(volatile size_t w) {
	while (w--) ;
}

__attribute__((flatten, noreturn, nothrow, cold))
void main(void) {
	for (int i = 0; i < 2; i++) {
		// FIRST WRITE GPIO
		MB_WR;
		gpio_act_on();
		wait(BLINK_WAIT);

		gpio_act_off();
		wait(BLINK_WAIT);
	}

	uart_init();
	uart_write(0x01);
	uart_write(0x23);
	uart_write(0x45);
	uart_write(0x67);
	uart_write(0x89);
	uart_write(0xab);
	uart_write(0xcd);
	uart_write(0xef);
	uart_read();

	while (1) asm volatile ("wfe");
}
