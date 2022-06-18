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

typedef void (*write_fun_t)(uint8_t, void*);

// write_fun is a func ptr that takes the character to write and a ptr to auxilliary data
// originally i was planning on using nested functions to keep track of aux data but
// that proved clunky and also not portable (although portability isnt really a priority)
// the new solution is also kinda clunky but now in an implementation way rather than
// a "gotta deal with the compiler" way
static int vdprintf(write_fun_t write_fun, void* aux, const char* fmt, va_list va) {
	while (*fmt) write_fun(*fmt++, aux);

	(void)va;
	return 0;
}

// ---------- printf ----------

static void printf_write(uint8_t c, void* aux) {
	uart_write(c);
	(void)aux;
}

int printf(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);

	int ret = vdprintf(printf_write, NULL, fmt, va);

	va_end(va);

	return ret;
}

// ---------- sprintf ----------

struct sprintf_data {
	char* buf;
	size_t pos;
};

static void sprintf_write(uint8_t c, void* aux) {
	struct sprintf_data* data = aux;

	data->buf[data->pos++] = c;
}

int sprintf(char* buf, const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);

	struct sprintf_data data = {.buf=buf, .pos=0};
	int ret = vdprintf(sprintf_write, &data, fmt, va);

	va_end(va);

	return ret;
}

// ---------- dprintf ----------

int dprintf(void (*write_fun)(uint8_t), const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);

	int ret = vdprintf((write_fun_t)(void*)write_fun, NULL, fmt, va);

	va_end(va);

	return ret;
}

// ---------- snprintf ----------

struct snprintf_data {
	char* buf;
	size_t n;
	size_t pos;
};

static void snprintf_write(uint8_t c, void* aux) {
	struct snprintf_data* data = aux;

	if (data->pos >= data->n - 1)
		data->buf[data->n - 1] = 0;
	else
		data->buf[data->pos++] = c;
}

int snprintf(char* buf, size_t size, const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);

	struct snprintf_data data = {.buf=buf, .n=size, .pos=0};
	int ret = vdprintf(snprintf_write, &data, fmt, va);

	va_end(va);

	return ret;
}
