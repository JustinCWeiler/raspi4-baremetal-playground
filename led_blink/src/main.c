#include "rpi.h"

#include "gpio.h"
#include "timer.h"

void main(void) {
	gpio_set_output(21);

	for (int i = 0; i < 4; i++) {
		gpio_set_on(21);
		delay_sec(1);
		gpio_set_off(21);
		delay_sec(1);
	}
}
