#include "rpi.h"
#include "mailbox.h"

// as of right now we don't have many utilities
// so we have a simple implementation
// this will be made better later

void mailbox_write(mailbox_msg_t* msg, uint8_t channel) {
	uintptr_t msg_ = (uintptr_t)msg;
	if (msg_ & 0xf)
		return;
	if (channel > 9)
		return;
	msg_ |= channel;

	while (GET32(MAIL_STATUS) & MAIL_FULL) ;

	PUT32(MAIL_WR, msg_);
}

mailbox_msg_t* mailbox_read(uint8_t channel) {
	if (channel > 9)
		return NULL;

	uintptr_t msg_;
	uint8_t channel_;
	do {
		while (GET32(MAIL_STATUS) & MAIL_EMPTY) ;

		msg_ = GET32(MAIL_RD);
		channel_ = msg_ & 0xf;
	} while (channel_ != channel);

	msg_ &= ~0xf;
	return (mailbox_msg_t*)msg_;
}

void mailbox_write_read(mailbox_msg_t* msg, uint8_t channel) {
	mailbox_write(msg, channel);

	while (mailbox_read(channel) != msg) ;
}
