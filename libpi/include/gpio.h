#ifndef _GPIO_H_
#define _GPIO_H_

//#include "rpi.h"

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
