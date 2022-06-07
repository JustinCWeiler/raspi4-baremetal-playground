#include "rpi.h"

#include "crc32.h"

uint32_t crc32(const void* void_buf, size_t n) {
	const uint8_t* buf = void_buf;
	uint32_t crc = ~0;
	for (size_t i = 0; i < n; i++) {
		uint8_t val = buf[i];
		asm volatile ("crc32b %w0, %w0, %w1" : "=&r" (crc) : "r" (val));
	}
	return ~crc;
}
