#include <stdint.h>
#include <stddef.h>

#define PERIPHERAL_BASE	0xFE000000

#define MB_RDWR		asm volatile ("dsb sy")
#define MB_RD		asm volatile ("dsb ld")
#define MB_WR		asm volatile ("dsb st")

#include "gpio.c"
#include "uart.c"
#include "timer.c"

#define BLINK_WAIT	0xC0000
#define TIMEOUT_WAIT	300000

#define SUCCESS		0x00
#define GET_INFO	0x11
#define PUT_INFO	0x22
#define GET_CODE	0x33
#define PUT_CODE	0x44

#define CRC_FAIL	0xF0
#define TIMEOUT		0xF1
#define MISC_FAIL	0xFF

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

	uint8_t recv;

	// GET_INFO loop
	while (1) {
		uart_write(GET_INFO);
		uint64_t now = timer_get_usec();
		while (timer_get_usec() - now < TIMEOUT_WAIT)
			if (uart_can_read())
				goto out;
	}
	out:

	// receive PUT_INFO
	recv = uart_read_raw();
	// TODO check recv is PUT_INFO

	// receive nbytes
	uint32_t nbytes = 0;
	for (int i = 0; i < 32; i += 8) {
		nbytes |= uart_read() << i;
	}

	// receive crc32
	uint32_t crc32 = 0;
	for (int i = 0; i < 32; i += 8) {
		crc32 |= uart_read() << i;
	}

	// send GET_CODE
	uart_write(GET_CODE);

	// send crc32
	for (int i = 0; i < 32; i += 8) {
		uart_write(crc32 >> i);
	}

	// receive PUT_CODE
	recv = uart_read();
	// TODO check recv is PUT_CODE

	// receive code bytes
	uint8_t* code = (void*)0x80000;
	uint32_t cur_crc = ~0;
	for (uint32_t i = 0; i < nbytes; i++) {
		uint8_t val = uart_read();
		code[i] = val;
		asm volatile ("crc32b %w0, %w0, %w1" : "+r" (cur_crc) : "r" (val) );
	}
	cur_crc = ~cur_crc;

	// check crc
	if (crc32 != cur_crc) {
		uart_write(CRC_FAIL);

		for (int i = 0; i < 32; i += 8) {
			uart_write(cur_crc >> i);
		}

		while (1) {
			gpio_act_on();
			wait(BLINK_WAIT);
			gpio_act_off();
			wait(BLINK_WAIT);
		}
	}

	// success!
	uart_write(SUCCESS);
	uart_flush();

	asm volatile ("br %0" :: "r" (code));

	while (1) asm volatile ("wfe");
	// TODO
	(void)recv;
}
