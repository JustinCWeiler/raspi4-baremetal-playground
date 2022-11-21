#include "rpi.h"

#include "gpio.h"
#include "uart.h"

#define AMOUNT 0x100000

void send_msg( const char* msg ) {
	while ( *msg ) uart_write( *msg++ );
}

void main( void ) {
	volatile int count;

	for ( int i = 0; i < 4; i++ ) {
		gpio_act_on();
		send_msg( "gpio act on\n" );
		count = AMOUNT;
		while ( count-- )
			;
		gpio_act_off();
		send_msg( "gpio act off\n" );
		count = AMOUNT;
		while ( count-- )
			;
	}
}
