.section ".data.lib"

GPIO_FSEL0: .word 0xFE200000
/*
unneeded
GPIO_FSEL1: .word 0xFE200004
GPIO_FSEL2: .word 0xFE200008
GPIO_FSEL3: .word 0xFE20000C
*/

GPIO_SET0:  .word 0xFE20001C
GPIO_SET1:  .word 0xFE200020
GPIO_CLR0:  .word 0xFE200028
GPIO_CLR1:  .word 0xFE20002C

GPIO_LEV0:  .word 0xFE200034

.section ".text.lib"

#include "asm-defs.S"

// all these functions assume inputs are correct
// any error that happens as a result of incorrect input parameters are all the user's fault etc etc

// void gpio_set_func(u32 pin, u32 func)
// set pin to func as specified on p67 of BCM2711
MK_FN(gpio_set_func)
	mov w2, wzr
gsf.loop:
	subs w0, w0, #10
	cinc w2, w2, ge
	b.ge gsf.loop
	add w0, w0, #10

	// w0 now holds offset
	// w1 holds func value
	// w2 now holds bank
	mov w3, #3
	mul w0, w0, w3
	lsl w0, w1, w0

	lsl w2, w2, #2

	// w0 now holds shifted value
	// w2 now holds bank*4 (offset value)
	// w3 now holds shift amount

	mov w1, #0b111
	lsl w1, w1, w3

	// w1 now holds bic value

	// rmw with w4
	ldr w4, GPIO_FSEL0
	ldr w4, [x4, x2]
	bic w4, w4, w1
	orr w4, w4, w0
	ldr w0, GPIO_FSEL0
	str w4, [x0, x2]

	ret

// void gpio_set_output(u32 pin)
// set pin to output
MK_FN(gpio_set_output)
	PUSH(x30, xzr)

	mov w1, #0b001
	bl gpio_set_func

	POP(x30, xzr)
	ret

// void gpio_set_input(u32 pin)
// set pin to input
MK_FN(gpio_set_input)
	PUSH(x30, xzr)

	mov w1, #0b000
	bl gpio_set_func

	POP(x30, xzr)
	ret

// void gpio_set_on(u32 pin)
// set pin on
// assumes pin already in output mode
MK_FN(gpio_set_on)
	mov w1, #1
	lsl w0, w1, w0
	ldr w1, GPIO_SET0
	str w0, [x1]
	ret

// void gpio_set_off(u32 pin)
// set pin off
// assumes pin already in output mode
MK_FN(gpio_set_off)
	mov w1, #1
	lsl w0, w1, w0
	ldr w1, GPIO_CLR0
	str w0, [x1]
	ret

// u32 gpio_read(u32 pin)
// return the value of pin
// assumes pin already in input mode
MK_FN(gpio_read)
	mov w0, wzr
	ret

// void gpio_act_on(void)
// turns on the act LED
MK_FN(gpio_act_on)
	ldr w0, GPIO_SET1
	mov w1, #1<<10
	str w1, [x0]
	ret

// void gpio_act_off(void)
// turns off the act LED
MK_FN(gpio_act_off)
	ldr w0, GPIO_CLR1
	mov w1, #1<<10
	str w1, [x0]
	ret
