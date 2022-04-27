#ifndef _MAILBOX_H_
#define _MAILBOX_H_

//#include "rpi.h"

#define MAIL_BASE (PERIPHERAL_BASE+0xB880)

#define MAIL_RD (MAIL_BASE)
#define MAIL_WR (MAIL_BASE+0x20)
#define MAIL_STATUS (MAIL_BASE+0x18)

#define MAIL_FULL (1<<31)
#define MAIL_EMPTY (1<<30)
#define MAIL_RESPONSE (1<<31)

// we only ever use channel 8
#define MAIL_CHANNEL 8

// later when we make a malloc we will make a function
// to generate this for us
typedef struct mailbox_msg{
	uint32_t size;
	uint32_t code;
	uint32_t tags[];
} mailbox_msg_t;

// writes `msg` to mailbox channel `channel`
void mailbox_write(mailbox_msg_t* msg, uint8_t channel);

// reads from mailbox channel `channel`
mailbox_msg_t* mailbox_read(uint8_t channel);

// writes `msg` to mailbox channel `channel`
// then blocks until response is same as `msg`
void mailbox_write_read(mailbox_msg_t* msg, uint8_t channel);

#endif
