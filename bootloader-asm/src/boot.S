.section ".text.boot"

.global _start
_start:
	// figure out end of memory

	// copy bootloader

	// setup stack
	ldr w0, =_start
	mov sp, x0

	ldr w0, =_boot_start_
	br x0
	// never return from main
