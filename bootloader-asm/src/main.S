.section ".text"

#include "asm-defs.S"

MK_FN(main)
	mov x0, #21
	bl gpio_set_output

loop:
	mov x0, #21
	bl gpio_set_on

	mov x0, #0x100000
	bl wait

	mov x0, #21
	bl gpio_set_off

	mov x0, #0x100000
	bl wait

	b loop

	ret

wait:
	subs x0, x0, #1
	bgt wait
	ret
