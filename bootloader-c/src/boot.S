.section ".text"

.global _start
_start:
	mov w0, #0x80000
	mov sp, x0

	b _cstart
