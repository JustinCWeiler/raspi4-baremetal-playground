.section ".data"

#define SUCCESS  #0x00
#define GET_INFO #0x11
#define PUT_INFO #0x22
#define GET_CODE #0x33
#define PUT_CODE #0x44

#define CRC_FAIL #0xF0
#define TIMEOUT  #0xF1

.section ".text"

#include "asm-defs.S"

#define BLINKYT(n, t) .rept(n); bl gpio_act_on; mov w0, t; bl timer_wait_ms; bl gpio_act_off; mov w0, t; bl timer_wait_ms; .endr
#define BLINKY(n) BLINKYT(n, #100)

MK_FN(main)
	PUSH(x30, xzr)

	// two blink
	BLINKY(2)

	// start
	bl uart_init
	
	mov w0, #0x22
	bl uart_write
	mov w0, #0x22
	bl uart_write
	mov w0, #0x11
	bl uart_write
	mov w0, #0x11
	mov w1, #1000
	bl send_recv

	b fast_blinky

	POP(x30, xzr)
	ret

slow_blinky:
	BLINKYT(1, #1000)
	b slow_blinky

fast_blinky:
	BLINKYT(1, #100)
	b fast_blinky
