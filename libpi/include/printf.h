#ifndef _PRINTF_H_
#define _PRINTF_H_

//#include "rpi.h"

int puts(const char* src);

int printf(const char* fmt, ...) __attribute__((format (printf, 1, 2)));
int sprintf(char* buf, const char* fmt, ...) __attribute__((format (printf, 2, 3)));
int dprintf(void (*write_fun)(uint8_t), const char* fmt, ...) __attribute__((format (printf, 2, 3)));
int snprintf(char* buf, size_t size, const char* fmt, ...) __attribute__((format (printf, 3, 4)));

#endif
