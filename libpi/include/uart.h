#ifndef _UART_H_
#define _UART_H_

//#include "rpi.h"

#define AUX_BASE	(PERIPHERAL_BASE+0x00215000)
#define AUX_MU_BASE	(AUX_BASE+0x40)

#define AUX_ENABLES	(AUX_BASE+0x04)
#define AUX_MU_IO	(AUX_MU_BASE+0x40)
#define AUX_MU_IER	(AUX_MU_BASE+0x44)
#define AUX_MU_IIR	(AUX_MU_BASE+0x48)
#define AUX_MU_LCR	(AUX_MU_BASE+0x4C)
#define AUX_MU_CNTL	(AUX_MU_BASE+0x60)
#define AUX_MU_STAT	(AUX_MU_BASE+0x64)
#define AUX_MU_BAUD	(AUX_MU_BASE+0x68)

// initialize uart to baudrate `baud`
// note that `baud` isn't the baudrate, but rather the value to set the buad register to
void uart_init_baud(uint32_t baud);
// initialize uart to default baudrate (115200)
void uart_init(void);

// write byte to uart
void uart_write(uint8_t data);
// read byte from uart
uint8_t uart_read(void);

// check whether we can write
unsigned uart_can_write(void);
// check whether we can read
unsigned uart_can_read(void);

#endif
