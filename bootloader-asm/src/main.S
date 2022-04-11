.section ".data"

#define SUCCESS  #0x00
#define GET_INFO #0x11
#define PUT_INFO #0x22
#define GET_CODE #0x33
#define PUT_CODE #0x44

#define CRC_FAIL #0xF0
#define TIMEOUT  #0xF1

#define TIMEOUT_VAL #300

#define CODE_LOC #0x80000

.section ".bss"

crc: .word

.section ".text.bootmain"

#include "asm-defs.S"

// debug defines
#define BLINKYT(n, t) .rept(n); bl gpio_act_on; mov w0, t; bl timer_wait_ms; bl gpio_act_off; mov w0, t; bl timer_wait_ms; .endr
#define BLINKY(n) BLINKYT(n, #100)

MK_FN(boot_main)
	// setup stack
	// we need to do this here too because this function may
	// be called by user programs to fake reboot
	mov x0, CODE_LOC
	mov sp, x0

	// two blink to signify start
	BLINKY(2)

	// send_recv GET_INFO, PUT_INFO
get_info.loop:
	mov w0, GET_INFO
	mov w1, TIMEOUT_VAL
	bl send_recv
	tst w0, #0xff00
	bnz get_info.loop

	// receive nbytes
	mov w19, #0
	bl uart_read
	bfi w19, w0, #0, #8
	bl uart_read
	bfi w19, w0, #8, #8
	bl uart_read
	bfi w19, w0, #16, #8
	bl uart_read
	bfi w19, w0, #24, #8

	// receive crc32
	mov w20, #0
	bl uart_read
	bfi w20, w0, #0, #8
	bl uart_read
	bfi w20, w0, #8, #8
	bl uart_read
	bfi w20, w0, #16, #8
	bl uart_read
	bfi w20, w0, #24, #8

	// (store crc32)
	ldr w0, =crc
	str w20, [x0]

	// send crc32
	mov w0, #0
	bfxil w0, w20, #0, #8
	bl uart_write
	bfxil w0, w20, #8, #8
	bl uart_write
	bfxil w0, w20, #16, #8
	bl uart_write
	bfxil w0, w20, #24, #8
	bl uart_write

	// send GET_CODE
	mov w0, GET_CODE
	bl uart_write

	// receive PUT_CODE
	bl uart_read

	// receive code bytes
	mov w20, CODE_LOC
	mov w21, #0
code_loop:
	bl uart_read
	str w0, [w20, w21]
	add w21, w21, #1
	cmp w21, w19
	blo code_loop

	// send SUCCESS
	mov w0, SUCCESS
	bl uart_write

	// branch to code
	mov w0, #0x80000
	br x0
	// NORETURN

// debug functions
slow_blinky:
	BLINKYT(1, #1000)
	b slow_blinky

fast_blinky:
	BLINKYT(1, #100)
	b fast_blinky
