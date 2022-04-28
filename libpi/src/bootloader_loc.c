#include "rpi.h"

#include "mailbox.h"

static mailbox_msg_t msg __attribute__((aligned(4)))= {
	8*4,
	0,
	{
		8,
		0,
		0,
		0,
		0x0
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
	if (bootloader_loc)
		bootloader_loc();

	get_bootloader_loc()();
}
