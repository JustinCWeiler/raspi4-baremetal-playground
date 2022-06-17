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
#define CODE_MISMATCH	0xF1
#define MISC_FAIL	0xFF

static void wait(volatile size_t w) {
	while (w--) ;
}

#define blink_error()\
		MB_WR;\
		while (1) {\
			gpio_act_on();\
			wait(BLINK_WAIT);\
			gpio_act_off();\
			wait(BLINK_WAIT);\
		}

static int get_info_timeout(void) {
		// UART LAST READ
		// UART FIRST WRITE
		MB_WR;
		uart_write(GET_INFO);
		MB_RD;
		// TIMER READ
		uint64_t now = timer_get_usec();
		// TIMER LAST READ
		while (timer_get_usec() - now < TIMEOUT_WAIT) {
			MB_RD;
			// UART LAST READ
			if (uart_can_read())
				return 0;
			MB_RD;
		}
		return 1;
}

__attribute__((flatten, noreturn, nothrow, cold))
void main(void) {
	top:

	for (int i = 0; i < 2; i++) {
		MB_WR;
		// GPIO WRITE
		gpio_act_on();
		wait(BLINK_WAIT);

		// GPIO WRITE
		gpio_act_off();
		wait(BLINK_WAIT);
	}

	gpio_act_on();

	uart_init();

	uint8_t recv;

	// GET_INFO loop
	while (get_info_timeout()) ;

	gpio_act_off();

	// receive PUT_INFO
	// UART READ
	recv = uart_read_raw();
	if (recv != PUT_INFO) {
		// UART READ
		// UART WRITE
		uart_write(CODE_MISMATCH);
		uart_write(PUT_INFO);
		uart_write(recv);
		// TODO what to do here?
		goto top;
	}

	// receive nbytes
	uint32_t nbytes = 0;
	for (int i = 0; i < 32; i += 8) {
		// UART READ
		nbytes |= uart_read() << i;
	}

	// receive crc32
	uint32_t crc32 = 0;
	for (int i = 0; i < 32; i += 8) {
		// UART READ
		crc32 |= uart_read() << i;
	}

	// send GET_CODE
	// UART READ
	// UART FIRST WRITE
	MB_WR;
	uart_write(GET_CODE);

	// send crc32
	for (int i = 0; i < 32; i += 8) {
		// UART READ
		// UART WRITE
		uart_write(crc32 >> i);
	}

	// receive PUT_CODE
	// UART READ
	recv = uart_read();
	if (recv != PUT_CODE) {
		// UART READ
		// UART WRITE
		uart_write(CODE_MISMATCH);
		uart_write(PUT_CODE);
		uart_write(recv);
		// TODO what to do here?
		goto top;
	}

	// receive code bytes
	uint8_t* code = (void*)0x80000;
	uint32_t cur_crc = ~0;
	for (uint32_t i = 0; i < nbytes; i++) {
		// UART READ
		uint8_t val = uart_read();
		code[i] = val;
		asm volatile ("crc32b %w0, %w0, %w1" : "+r" (cur_crc) : "r" (val) );
	}
	cur_crc = ~cur_crc;

	// check crc
	if (crc32 != cur_crc) {
		// UART READ
		// UART WRITE
		uart_write(CRC_FAIL);

		for (int i = 0; i < 32; i += 8) {
			// UART READ
			// UART WRITE
			uart_write(cur_crc >> i);
		}

		blink_error();
	}

	// success!
	// UART READ
	// UART WRITE
	uart_write(SUCCESS);
	// UART LAST READ
	uart_flush();
	MB_RD;

	asm volatile ("br %0" :: "r" (code));

	__builtin_unreachable();
}
