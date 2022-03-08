#include <blinky.h>

void blinky(void) {
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
