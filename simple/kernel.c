#include <stdint.h>

const uintptr_t P_BASE = 0xfe000000;
const uintptr_t GPIO_BASE = P_BASE + 0x200000;
const uintptr_t GPIO_FSEL2 = GPIO_BASE + 0x8;
const uintptr_t GPIO_SET0 = GPIO_BASE + 0x1c;
const uintptr_t GPIO_CLR0 = GPIO_BASE + 0x28;

extern void wait(uint64_t time);
extern void PUT32(uintptr_t addr, uint32_t val);
extern uint32_t GET32(uintptr_t addr);

#define TIME 0x100000
void main(void) {
	uint32_t val = GET32(GPIO_FSEL2);
	val &= ~(0b111<<3);
	val |= 0b001<<3;
	PUT32(GPIO_FSEL2, val);

	while (1) {
		PUT32(GPIO_SET0, 1 << 21);
		wait(TIME);
		PUT32(GPIO_CLR0, 1 << 21);
		wait(TIME);
	}
}
