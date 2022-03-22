.section ".data.lib"

AUX_ENABLES:    .word 0xFE215004
AUX_MU_IO:      .word 0xFE215040
AUX_MU_IER:     .word 0xFE215044
AUX_MU_IIR:     .word 0xFE215048
AUX_MU_LCR:     .word 0xFE21504C
AUX_MU_MCR:     .word 0xFE215050
AUX_MU_LSR:     .word 0xFE215054
AUX_MU_MSR:     .word 0xFE215058
AUX_MU_SCRATCH: .word 0xFE21505C
AUX_MU_CNTL:    .word 0xFE215060
AUX_MU_STAT:    .word 0xFE215064
AUX_MU_BAUD:    .word 0xFE215068

.section ".text.lib"

#include "asm-defs.S"

// void uart_init(void)
// initializes and enables uart
MK_FN(uart_init)
	ret

// void uart_write(u8 data)
// sends single byte of data
// blocks until can send
MK_FN(uart_write)
	ret

// u8 uart_read(void)
// reads single byte of data
// blocks until can read
MK_FN(uart_read)
	mov w0, #0
	ret
