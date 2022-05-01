#include "rpi.h"

#include "uart.h"
#include "gpio.h"

void main(void) {
	uart_init();

	return_to_bootloader();
}
