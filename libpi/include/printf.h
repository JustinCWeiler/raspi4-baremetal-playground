#ifndef _PRINTF_H_
#define _PRINTF_H_

//#include "rpi.h"

void puts(const char* src);

void printf(const char* fmt, ...) __attribute__((format (printf, 1, 2)));
void sprintf(char* buf, const char* fmt, ...) __attribute__((format (printf, 2, 3)));
void snprintf(char* buf, size_t size, const char* fmt, ...) __attribute__((format (printf, 3, 4)));

#endif
