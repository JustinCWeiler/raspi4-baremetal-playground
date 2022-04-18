#ifndef _LIBCRC_H_
#define _LIBCRC_H_

#include <stdint.h>
#include <stddef.h>

// takes a previous crc value and byte of data and returns next crc value
uint32_t crc_increment(uint32_t prev_crc, uint8_t data);

// returns crc32 value of byte array data
uint32_t crc32(uint8_t* data, size_t n);

#endif
