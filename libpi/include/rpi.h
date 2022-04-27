#ifndef _RPI_H_
#define _RPI_H_

#include <stdint.h>
#include <stddef.h>

#define PERIPHERAL_BASE 0xFE000000

#define GET32(addr) ({							\
	volatile uint32_t* addr_ = (uint32_t*)(uintptr_t)(addr);	\
	*addr_;								\
})

#define PUT32(addr, val) do {						\
	uint32_t val_ = (uint32_t)(val);				\
	volatile uint32_t* addr_ = (uint32_t*)(uintptr_t)(addr);	\
	*addr_ = val_;							\
} while(0)

#define MB_RDWR (asm volatile ( "dsb sy" :::));
#define MB_RD (asm volatile ( "dsb ld" :::));
#define MB_WR (asm volatile ( "dsb st" :::));

#endif
