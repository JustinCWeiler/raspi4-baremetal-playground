#ifndef _GPIO_H_
#define _GPIO_H_

//#include "rpi.h"

#define GPIO_BASE	(PERIPHERAL_BASE+0x00200000)

#define GPIO_FSEL0	(GPIO_BASE+0x00)
#define GPIO_FSEL1	(GPIO_BASE+0x04)
#define GPIO_FSEL2	(GPIO_BASE+0x08)
#define GPIO_FSEL3	(GPIO_BASE+0x0C)
#define GPIO_FSEL4	(GPIO_BASE+0x10)
#define GPIO_FSEL5	(GPIO_BASE+0x14)

#define GPIO_SET0	(GPIO_BASE+0x1C)
#define GPIO_SET1	(GPIO_BASE+0x20)
#define GPIO_CLR0	(GPIO_BASE+0x28)
#define GPIO_CLR1	(GPIO_BASE+0x2C)

#define GPIO_LEV0	(GPIO_BASE+0x34)
#define GPIO_LEV1	(GPIO_BASE+0x34)

#define GPIO_INPUT	0b000
#define GPIO_OUTPUT	0b001
#define GPIO_FUNC0	0b100
#define GPIO_FUNC1	0b101
#define GPIO_FUNC2	0b110
#define GPIO_FUNC3	0b111
#define GPIO_FUNC4	0b011
#define GPIO_FUNC5	0b010

// set `pin` to 3 bit value `func`
void gpio_set_func(uint32_t pin, uint32_t func);

// set `pin` to output
void gpio_set_output(uint32_t pin);
// set `pin` to input
void gpio_set_input(uint32_t pin);

// turn `pin` on
void gpio_set_on(uint32_t pin);
// turn `pin` off
void gpio_set_off(uint32_t pin);

// read the value of `pin`
unsigned gpio_read(uint32_t pin);

// turn act led on
void gpio_act_on(void);
// turn act led off
void gpio_act_off(void);

#endif
