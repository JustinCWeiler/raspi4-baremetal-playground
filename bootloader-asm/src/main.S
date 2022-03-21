.section ".text"

#include "asm-defs.S"

MK_FN(main)
	PUSH(x30, xzr)

loop:
	bl gpio_act_on
	mov x0, #1
	bl timer_wait

	bl gpio_act_off
	mov x0, #1
	bl timer_wait
	
	b loop

	POP(x30, xzr)
	ret
