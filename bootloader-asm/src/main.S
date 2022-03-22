.section ".data"

CYCLE_DATA:
.byte 0x12
.byte 0x34
.byte 0x56
.byte 0x78
.byte 0x9a
.byte 0xbc
.byte 0xde
.byte 0xff

.section ".text"

#include "asm-defs.S"

#define BLINKYT(n, t) .rept(n); bl gpio_act_on; mov w0, t; bl timer_wait_ms; bl gpio_act_off; mov w0, t; bl timer_wait_ms; .endr
#define BLINKY(n) BLINKYT(n, #100)

MK_FN(main)
	PUSH(x30, x19)

	// two blink
	BLINKY(2)

	// start
	bl uart_init
loop:

	// deadbeef
	mov w0, #0xef
	bl uart_write
	mov w0, #0xbe
	bl uart_write
	mov w0, #0xad
	bl uart_write
	mov w0, #0xde
	bl uart_write

	mov w0, #300
	bl timer_wait_ms

	b loop

	POP(x30, x19)
	ret

bad_blinky:
	BLINKY(1)
	b bad_blinky
