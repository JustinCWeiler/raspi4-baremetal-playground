#include "rpi.h"
#include "gpio.h"

void gpio_set_func(uint32_t pin, uint32_t func) {
	if (func > 0b111)
		return;
	if (pin > 57)
		return;

	uintptr_t fsel = GPIO_FSEL0 + (pin / 10)*4;
	unsigned offset = pin % 10;

	// LAST READ
	uint32_t val = GET32(fsel);
	val &= ~(0b111 << offset);
	val |= func;
	MB_RDWR;
	// FIRST WRITE
	PUT32(fsel, val);
}

void gpio_set_output(uint32_t pin) {
	gpio_set_func(pin, GPIO_OUTPUT);
}

void gpio_set_input(uint32_t pin) {
	gpio_set_func(pin, GPIO_INPUT);
}

void gpio_set_on(uint32_t pin) {
	if (pin > 57)
		return;

	// FIRST WRITE
	MB_WR;
	if (pin > 31)
		PUT32(GPIO_SET0, 1 << pin);
	else
		PUT32(GPIO_SET1, 1 << (pin - 32));
}

void gpio_set_off(uint32_t pin) {
	if (pin > 57)
		return;

	// FIRST WRITE
	MB_WR;
	if (pin <= 31)
		PUT32(GPIO_CLR0, 1 << pin);
	else
		PUT32(GPIO_CLR1, 1 << (pin - 32));
}

unsigned gpio_read(uint32_t pin) {
	if (pin > 57)
		return 0;

	unsigned ret;
	// LAST READ
	if (pin <= 31)
		ret = GET32(GPIO_LEV0) >> pin;
	else
		ret = GET32(GPIO_LEV1) >> (pin - 32);
	MB_RD;
	return ret & 1;
}

void gpio_act_on(void) {
	// FIRST WRITE
	MB_WR;
	PUT32(GPIO_SET1, 1<<10);
}

void gpio_act_off(void) {
	// FIRST WRITE
	MB_WR;
	PUT32(GPIO_CLR1, 1<<10);
}
