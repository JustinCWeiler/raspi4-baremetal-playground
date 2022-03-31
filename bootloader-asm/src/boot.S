.section ".text.boot"

.global _start
_start:
	ldr w0, =_start
	mov sp, x0

	b main
	// never return from main
