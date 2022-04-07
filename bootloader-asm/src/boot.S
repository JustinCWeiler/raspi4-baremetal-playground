.section ".text.boot"

#include "asm-defs.S"

.global _start
_start:
	// setup stack
	// technically not needed here because get_end_mem
	// never uses the stack, but still do it cuz proper
	ldr w0, =_start
	mov sp, x0

	// figure out end of memory
	bl get_end_mem
	// w0 contains end memory addr + 1
	ldr w1, =_boot_end_
	// w1 contains end prog addr + 1
	ldr w2, =_boot_start_
	sub w2, w1, w2
	// w2 contains prog size in bytes
	sub w0, w0, #4
	sub w0, w0, w2
	bfi w0, wzr, #0, #4
	// w0 contains start memory addr
	// rounded down to nearest 16 byte boundary

	// do the copy
copy.loop:
	subs w2, w2, #1
	sub w1, w1, #1
	ldrb w3, [x1]
	strb w3, [x0, x2]
	bne copy.loop

	MB_WR

	// branch
	br x0
	// never return here
