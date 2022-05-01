.section ".data.lib"

MAIL_RD: .word 0xFE00B880
MAIL_WR: .word 0xFE00B8A0
MAIL_STATUS: .word 0xFE00B880+0x18

// bit set if cannot write
#define MAIL_FULL #1<<31
// bit set if nothing to read
#define MAIL_EMPTY #1<<30
// bit set if response successful
#define MAIL_RESPONSE #1<<31
// channel we use
#define MAIL_CHANNEL #8

.section ".text.lib"

#include "asm-defs.S"

// must be 16-byte aligned
.balign(16)
msg_mem_buffer:
// u32: buffer size in bytes
.word 8*4		// 0
// u32: buffer request/response code: sbz on request
//      0x80000000: request successful
//      0x80000001: error parsing request buffer (partial response)
.word 0			// 1
// tag
// u32: tag identifier
.word 0x00010005	// 2
// u32 response size in bytes
.word 8			// 3
// u32 request size in bytes
.word 0			// 4
// u32... value buffer
.word 0			// 5 base address
.word 0			// 6 size
// u32 0x00000000: end tag
.word 0			// 7

// void get_end_mem(void)
MK_FN(get_end_mem)
	MB_WR

	// read status to see if we can write
	ldr w1, MAIL_STATUS
gms.can_write_loop:
	ldr w0, [x1]
	tst w0, MAIL_FULL
	bnz gms.can_write_loop

	// do the write
	ldr w1, MAIL_WR
	ldr w0, =msg_mem_buffer
	// channel 8
	orr w0, w0, MAIL_CHANNEL
	str w0, [x1]

	// wait for reply
gms.get_reply:
	ldr w1, MAIL_STATUS
gms.can_read_loop:
	ldr w0, [x1]
	tst w0, MAIL_EMPTY
	bnz gms.can_read_loop

	// read the response
	ldr w1, MAIL_RD
	ldr w0, [x1]
	ldr w1, =msg_mem_buffer
	orr w1, w1, #8
	cmp w0, w1
	bne gms.get_reply

	bfi w1, wzr, #0, #4
	ldr w0, [x1, #5*4]
	ldr w1, [x1, #6*4]
	add w0, w0, w1

	MB_RD

	ret
