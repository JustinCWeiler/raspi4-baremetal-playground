#include "rpi.h"

#include "printf.h"
#include "uart.h"

#include "stdarg.h"

int puts(const char* src) {
	int i = 0;
	while (*src) {
		uart_write(*src++);
		i++;
	}

	return i;
}

static int vdprintf(void (*write_fun)(uint8_t), const char* fmt, va_list va) {
}

int printf(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);

	int ret = vdprintf(uart_write, fmt, va);

	va_end(va);

	return ret;
}

int sprintf(char* buf, const char* fmt, ...) {
	void write(uint8_t c) {
		*buf++ = c;
	}
}

int dprintf(void (*write_fun)(uint8_t), const char* fmt, ...) {
}

int snprintf(char* buf, size_t size, const char* fmt, ...) {
}
