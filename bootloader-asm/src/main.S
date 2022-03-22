.section ".text"

#include "asm-defs.S"

MK_FN(main)
	PUSH(x30, xzr)

	bl two_blink

	mov w0, #21
	bl gpio_set_input

loop:
	mov w0, #21
	bl gpio_read

	cmp w0, #0
	b.eq zero
one:
	bl gpio_act_on
	b loop
zero:
	bl gpio_act_off
	b loop

	POP(x30, xzr)
	ret

two_blink:
	PUSH(x30, xzr)

	.rept(2)
	bl gpio_act_on
	mov x0, #100
	bl timer_wait_ms
	bl gpio_act_off
	mov x0, #100
	bl timer_wait_ms
	.endr

	POP(x30, xzr)
	ret
