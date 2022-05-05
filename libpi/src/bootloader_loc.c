#include "rpi.h"

#include "mailbox.h"
#include "uart.h"

static mailbox_msg_t msg __attribute__((aligned(16)))= {
	8*4,
	0,
	{
		0x00010005,	// tag identifier
		8,		// response size
		0,		// request size
		0,		// value buffer
		0,
		0x0		// end tag
	}
};

static boot_func_t bootloader_loc = NULL;

boot_func_t get_bootloader_loc(void) {
	if (bootloader_loc)
		return bootloader_loc;

	mailbox_write_read(&msg, MAIL_CHANNEL);

	uintptr_t base = msg.tags[3];
	uintptr_t size = msg.tags[4];
	uintptr_t end = base + size - 4;

	bootloader_loc = (boot_func_t)(uintptr_t)GET32(end);

	return bootloader_loc;
}

void return_to_bootloader(void) {
	uart_init();

	// 4 EOT characters to signal end of program
	// very very unlikely that program sends 4 EOTs in a row,
	// even in an erroneous state
	uart_write(0x04);
	uart_write(0x04);
	uart_write(0x04);
	uart_write(0x04);

	if (bootloader_loc)
		bootloader_loc();

	get_bootloader_loc()();
}
