.section ".text.boot"

.global _start
_start:
	// set stack
	ldr w0, =_start
	mov sp, x0

	// clear bss
	ldr w0, =__bss_start__
	ldr w1, =__bss_end__

bss_loop:
	str xzr, [x0]
	add x0, x0, #8
	cmp x0, x1
	blo bss_loop

	// init functions

	// uart
	bl uart_init

	// heap (default 1GB of space)
	ldr w0, =__bss_end__
	mov w1, w0
	add w1, w1, 1 << 20
	bl reset_heap

	bl main

	bl return_to_bootloader
