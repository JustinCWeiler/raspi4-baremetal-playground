#include <stdint.h>
#include <stddef.h>

#define PERIPHERAL_BASE	0xFE000000

#define MB_RDWR		asm volatile ("dsb sy")
#define MB_RD		asm volatile ("dsb ld")
#define MB_WR		asm volatile ("dsb st")

#include "gpio.c"
#include "uart.c"

#define BLINK_WAIT	0xC0000
#define TIMEOUT_WAIT	300000

#define SUCCESS		#0x00
#define GET_INFO	#0x11
#define PUT_INFO	#0x22
#define GET_CODE	#0x33
#define PUT_CODE	#0x44

#define CRC_FAIL	#0xF0
#define TIMEOUT		#0xF1
#define MISC_FAIL	#0xFF

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

	// GET_INFO loop

	// receive PUT_INFO

	// receive nbytes

	// receive crc32

	// send GET_CODE

	// send crc32

	// receive PUT_CODE

	// receive code bytes

	// check crc

	// success!

	while (1) asm volatile ("wfe");
}
