#include <stdint.h>
#include <stddef.h>

#define PERIPHERAL_BASE 0xFE000000
#define GPIO_BASE (PERIPHERAL_BASE+0x00200000)
#define GPIO_SET1 ((volatile uint32_t*)(GPIO_BASE+0x20))
#define GPIO_CLR1 ((volatile uint32_t*)(GPIO_BASE+0x2C))

static void gpio_act_on(void) {
	// WRITE
	*GPIO_SET1 = 1 << 10;
}

static void gpio_act_off(void) {
	// WRITE
	*GPIO_CLR1 = 1 << 10;
}

void main(void) __attribute__((flatten, noreturn, nothrow));
void main(void) {
	for (int i = 0; i < 2; i++) {
		gpio_act_on();
		volatile size_t w = 0xC0000;
		while (w--) ;

		gpio_act_off();
		w = 0xC0000;
		while (w--) ;
	}

	while (1) asm volatile ("wfe");
}
