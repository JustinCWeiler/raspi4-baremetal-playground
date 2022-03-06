.section ".text.boot"

.global _start
_start:
	ldr w0, =_start
	mov sp, x0

	bl main
hang:
	wfe
	b hang

.section ".text"

.global PUT32
PUT32:
	str w1, [x0]
	ret

.global GET32
GET32:
	ldr w0, [x0]
	ret

.global wait
wait:
	subs x0, x0, #1
	bne wait
	ret
