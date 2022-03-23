.section ".data.lib"

AUX_ENABLES:    .word 0xFE215004
AUX_MU_IO:      .word 0xFE215040
AUX_MU_IER:     .word 0xFE215044
AUX_MU_IIR:     .word 0xFE215048
AUX_MU_LCR:     .word 0xFE21504C
//AUX_MU_MCR:     .word 0xFE215050
//AUX_MU_LSR:     .word 0xFE215054
//AUX_MU_MSR:     .word 0xFE215058
//AUX_MU_SCRATCH: .word 0xFE21505C
AUX_MU_CNTL:    .word 0xFE215060
AUX_MU_STAT:    .word 0xFE215064
AUX_MU_BAUD:    .word 0xFE215068

.section ".text.lib"

#include "asm-defs.S"

// void uart_init(void)
// initializes and enables uart
MK_FN(uart_init)
	PUSH(x30, xzr)

	// set gpio pins
	mov w0, #14
	mov w1, 0b010 
	bl gpio_set_func
	mov w0, #15
	mov w1, 0b010 
	bl gpio_set_func

	MB_WR

	// rmw to enable uart
	ldr w1, AUX_ENABLES
	ldr w0, [x1]
	orr w0, w0, #0b001
	str w0, [x1]

	MB_RDWR

	// configure

	// disable tx and rx
	ldr w1, AUX_MU_CNTL
	mov w0, #0
	str w0, [x1]

	// clear FIFOs
	ldr w1, AUX_MU_IIR
	mov w0, #0b11<<1
	str w0, [x1]

	// disable interrupts
	ldr w1, AUX_MU_IER
	mov w0, #0
	str w0, [x1]

	// set baudrate
	ldr w1, AUX_MU_BAUD
	// f423
	mov w0, #541
	str w0, [x1]
	
	// set to 8 bit mode
	ldr w1, AUX_MU_LCR
	mov w0, #0b11
	str w0, [x1]

	// reenable tx and rx
	ldr w1, AUX_MU_CNTL
	mov w0, #0b11
	str w0, [x1]

	MB_RD

	POP(x30, xzr)
	ret

// void uart_write(u8 data)
// sends single byte of data
// blocks until can send
MK_FN(uart_write)
	// check if can write
	ldr w1, AUX_MU_STAT
uw.loop:
	ldr w2, [x1]
	tst w2, #1<<1
	b.eq uw.loop

	MB_RDWR

	ldr w1, AUX_MU_IO
	str w0, [x1]
	ret

// u8 uart_read(void)
// reads single byte of data
// blocks until can read
MK_FN(uart_read)
	// check if can read
	ldr w1, AUX_MU_STAT
ur.loop:
	ldr w2, [x1]
	tst w1, #1<<0
	b.eq ur.loop

	ldr w1, AUX_MU_IO
	ldr w0, [x1]

	MB_RD
	ret

// bool uart_can_write(void)
// returns whether or not we have space in tx FIFO
MK_FN(uart_can_write)
	ldr w1, AUX_MU_STAT
	ldr w0, [x1]
	lsr w0, #1
	and w0, #1

	MB_RD
	ret

// bool uart_can_read(void)
// returns whether or not we have data in rx FIFO
MK_FN(uart_can_read)
	ldr w1, AUX_MU_STAT
	ldr w0, [x1]
	and w0, #1

	MB_RD
	ret
