#include "libcrc.h"

#include <stdio.h>

#define POLYNOMIAL 0xEDB88320
static uint32_t crc_table[256];
static int is_generated = 0;

static void generate_table(void) {
	if (is_generated)
		return;

	for (int i = 0; i < 256; i++) {
		uint32_t val = i;
		for (int j = 0; j < 8; j++) {
			int lsb = val & 1;
			val >>= 1;
			if (lsb)
				val ^= POLYNOMIAL;
		}
		crc_table[i] = val;
	}

	is_generated = 1;
}

uint32_t crc_increment(uint32_t prev_crc, uint8_t data) {
	uint8_t index = prev_crc ^ data;
	return (prev_crc >> 8) ^ crc_table[index];
}

uint32_t crc32(const uint8_t* data, size_t n) {
	if (!is_generated)
		generate_table();

	uint32_t crc = ~0;

	for (int i = 0; i < n; i++) {
		crc = crc_increment(crc, data[i]);
	}

	return ~crc;
}
