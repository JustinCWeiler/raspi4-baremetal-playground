.section ".text.lib"

#include "asm-defs.S"

// all these functions assume inputs are correct
// any error that happens as a result of incorrect input parameters are all the user's fault etc etc

// void gpio_set_func(u64 pin, u64 func)
// set pin to alternative function func
MK_FN(gpio_set_func)
	ret

// void gpio_set_output(u64 pin)
// set pin to output
MK_FN(gpio_set_output)
	ret

// void gpio_set_input(u64 pin)
// set pin to input
MK_FN(gpio_set_input)
	ret

// void gpio_set_on(u64 pin)
// set pin on
// assumes pin already in output mode
MK_FN(gpio_set_on)
	ret

// void gpio_set_off(u64 pin)
// set pin off
// assumes pin already in output mode
MK_FN(gpio_set_off)
	ret

// u64 gpio_read(u64 pin)
// return the value of pin
// assumes pin already in input mode
MK_FN(gpio_read)
	mov x0, xzr
	ret
