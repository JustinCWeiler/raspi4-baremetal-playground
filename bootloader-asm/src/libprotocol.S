.section ".data.lib"

.section ".text.lib"

#include "asm-defs.S"

// u8 recv(u32 timeout)
// wait at most timeout ms to receive data
MK_FN(recv)
	PUSH(x30, x19)
	PUSH(x20, x21)

	mov w1, #1000
	mul w0, w0, w19

	mov w19, w0
	// w19 = timeout

	bl timer_get_us
	mov w20, w0
	// w20 = start time
r.loop:
	bl uart_can_read
	mov w21, w0
	// w21 = can read?
	bl timer_get_us
	sub w0, w0, w20
	// w0 = delta time

	// w21 = can read?
	cmp w21, #1
	beq r.after

	// w19 = timeout
	// w0 = delta time
	cmp w19, w0
	bhi r.loop

	mov w0, #0xffff
	b r.end

r.after:
	// read
	bl uart_read

r.end:
	POP(x20, x21)
	POP(x30, x19)
	ret

// u8 send_recv(u8 data, u32 timeout)
// send data and wait at most timeout ms to receive
MK_FN(send_recv)
	PUSH(x30, x19)

	mov w19, w1

	bl uart_write

	mov w0, w19
	bl recv

	POP(x30, x19)
	ret
