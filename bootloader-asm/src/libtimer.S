.section ".data.lib"

TIMER_CLO: .word 0xFE003004
/*
unneeded
TIMER_CHI: .word 0xFE003008
*/

.section ".text.lib"

#include "asm-defs.S"

// u32 timer_get_us(void)
// get the current timer count (in us)
MK_FN(timer_get_us)
	ldr w0, TIMER_CLO
	ldr w0, [x0]

	MB_RD
	ret

// void timer_wait_us(u32 us)
// wait for the specified number of microseconds
/*
MK_FN(timer_wait_us)
	ldr w3, TIMER_CLO
	ldr w1, [x3]
twu.loop:
	ldr w2, [x3]
	sub w2, w2, w1
	cmp w0, w2
	bhi twu.loop

	MB_RD
	ret

// void timer_wait_ms(u32 ms)
// wait for the specified number of milliseconds
MK_FN(timer_wait_ms)
	PUSH(x30, xzr)
	mov x1, #1000
	mul x0, x0, x1
	bl timer_wait_us
	POP(x30, xzr)
	ret

// void timer_wait(u32 sec)
// wait for the specified number of seconds
MK_FN(timer_wait)
	PUSH(x30, xzr)
	mov x1, #0x4240
	movk x1, #0x000f, lsl #0x10
	mul x0, x0, x1
	bl timer_wait_us
	POP(x30, xzr)
	ret
*/
