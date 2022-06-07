#include <stdint.h>
#include <stddef.h>

extern unsigned char _boot_start_;
extern unsigned char _boot_end_;

#define PERIPHERAL_BASE		0xFE000000
#define MAIL_BASE		(PERIPHERAL_BASE+0xB880)
#define MAIL_RD			((volatile uint32_t*)(MAIL_BASE))
#define MAIL_WR			((volatile uint32_t*)(MAIL_BASE+0x20))
#define MAIL_STATUS		((volatile uint32_t*)(MAIL_BASE+0x18))

#define CHANNEL			8

#define MAIL_FULL		(1<<31)
#define MAIL_EMPTY		(1<<30)

#define MB_RD			asm volatile ("dsb ld");
#define MB_WR			asm volatile ("dsb st");
#define MB_RDWR			asm volatile ("dsb sy");

static uint32_t msg[8] __attribute__((aligned(16))) = {
	// msg size
	8*4,
	// zero
	0,
	// tag id
	0x00010005,
	// response size
	8,
	// request size
	0,
	// value buffer
	0,
	0,
	// end tag
	0x0
};

static uintptr_t get_end_mem(void) {
	// write to mailbox
	uintptr_t m = (uintptr_t)(msg) | CHANNEL;

	// READ
	while (*MAIL_STATUS & MAIL_FULL) ;

	MB_WR;
	// FIRST WRITE
	*MAIL_WR = m;

	// read from mailbox
	do {
		uint8_t c;
		do {
			// READ
			while (*MAIL_STATUS & MAIL_EMPTY) ;

			// LAST READ
			m = *MAIL_RD;
			MB_RD;
			c = m & 0xf;
		} while (c != CHANNEL);
	} while ((m & ~0xf) != (uintptr_t)msg);

	uintptr_t base = msg[5];
	uintptr_t size = msg[6];
	return base + size;
}

__attribute__((flatten, noreturn, nothrow, cold))
void _cstart(void) {
	uintptr_t end = get_end_mem();
	// end - 4 = boot_loc
	uint32_t* boot_loc = (uint32_t*)end - 1;
	// (end - 4 - prog_size) & ~0xf = prog_start
	size_t prog_size = (size_t)&_boot_end_ - (size_t)&_boot_start_;
	uint8_t* prog_start = (uint8_t*)((end - 4 - prog_size) & ~0xf);

	uint8_t* prog_ptr = prog_start;
	uint8_t* boot_ptr = &_boot_start_;

	*boot_loc = (uint32_t)(uintptr_t)prog_start;

	while (prog_size--) *prog_ptr++ = *boot_ptr++;

	asm volatile ("br %0" :: "r" (prog_start));

	__builtin_unreachable();
}
