#ifndef _BLINKY_H_
#define _BLINKY_H_

#include <stdint.h>

#define P_BASE 0xfe000000
#define GPIO_BASE (P_BASE + 0x200000)
#define GPIO_FSEL2 (GPIO_BASE + 0x8)
#define GPIO_SET0 (GPIO_BASE + 0x1c)
#define GPIO_SET1 (GPIO_BASE + 0x20)
#define GPIO_CLR0 (GPIO_BASE + 0x28)
#define GPIO_CLR1 (GPIO_BASE + 0x2c)

extern void wait(uint64_t time);
extern void PUT32(uintptr_t addr, uint32_t val);
extern uint32_t GET32(uintptr_t addr);

#define TIME 0x100000

void blinky(void);

#endif
