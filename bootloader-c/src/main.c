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
	volatile size_t i;

	while (1) {
		gpio_act_on();
		i = 0x100000;
		while (i--) ;

		gpio_act_off();
		i = 0x100000;
		while (i--) ;
	}

	__builtin_unreachable();
}
