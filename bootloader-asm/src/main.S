.section ".text"

#include "asm-defs.S"

MK_FN(main)
	mov x0, #21
	bl gpio_set_output

	mov x0, #21
	bl gpio_set_on

	ret
