.section ".data"

#define SUCCESS		#0x00
#define GET_INFO	#0x11
#define PUT_INFO	#0x22
#define GET_CODE	#0x33
#define PUT_CODE	#0x44

#define CRC_FAIL	#0xF0
#define TIMEOUT		#0xF1
#define MISC_FAIL	#0xFF

#define TIMEOUT_VAL #300

#define CODE_LOC #0x80000

#define BLINKY_VAL #0xC0000

.section ".text.bootmain"

#include "asm-defs.S"

MK_FN(boot_main)
	// two blink to signify start
	bl gpio_act_on
	mov x0, BLINKY_VAL
wait1:
	sub x0, x0, #1
	cbnz x0, wait1

	bl gpio_act_off
	mov x0, BLINKY_VAL
wait2:
	sub x0, x0, #1
	cbnz x0, wait2


	bl gpio_act_on
	mov x0, BLINKY_VAL
wait3:
	sub x0, x0, #1
	cbnz x0, wait3

	bl gpio_act_off
	mov x0, BLINKY_VAL
wait4:
	sub x0, x0, #1
	cbnz x0, wait4

	// setup stack
	// we need to do this here too because this function may
	// be called by user programs to fake reboot
	mov x0, CODE_LOC
	mov sp, x0

	bl uart_init

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
	mov w23, #0
	bl uart_read
	bfi w23, w0, #0, #8
	bl uart_read
	bfi w23, w0, #8, #8
	bl uart_read
	bfi w23, w0, #16, #8
	bl uart_read
	bfi w23, w0, #24, #8

	// send GET_CODE
	mov w0, GET_CODE
	bl uart_write

	// send crc32
	mov w0, #0
	bfxil w0, w23, #0, #8
	bl uart_write
	bfxil w0, w23, #8, #8
	bl uart_write
	bfxil w0, w23, #16, #8
	bl uart_write
	bfxil w0, w23, #24, #8
	bl uart_write

	// receive PUT_CODE
	bl uart_read

	// receive code bytes
	mov w20, CODE_LOC
	mov w21, #0
	movn w22, #0
code_loop:
	bl uart_read
	strb w0, [x20, x21]
	crc32b w22, w22, w0
	add w21, w21, #1
	cmp w21, w19
	blo code_loop

	mvn w22, w22
	cmp w22, w23
	beq succ

	mov w0, CRC_FAIL
	bl uart_write

	bfxil w0, w23, #0, #8
	bl uart_write
	bfxil w0, w23, #8, #8
	bl uart_write
	bfxil w0, w23, #16, #8
	bl uart_write
	bfxil w0, w23, #24, #8
	bl uart_write

	b fast_blinky

succ:
	// send SUCCESS
	mov w0, SUCCESS
	bl uart_write

succ.wait_done:
	bl uart_tx_done
	cmp w0, #1
	bne succ.wait_done

	// branch to code
	mov w0, CODE_LOC
	br x0
	// NORETURN

fast_blinky:
	bl gpio_act_on
	mov x0, BLINKY_VAL
fb_wait1:
	sub x0, x0, #1
	cbnz x0, fb_wait1
	bl gpio_act_off
	mov x0, BLINKY_VAL
fb_wait2:
	sub x0, x0, #1
	cbnz x0, fb_wait2

	b fast_blinky
