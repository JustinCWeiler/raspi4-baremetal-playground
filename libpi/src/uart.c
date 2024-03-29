#include "rpi.h"
#include "uart.h"

#include "gpio.h"

#define AUX_BASE ( PERIPHERAL_BASE + 0x00215000 )
#define AUX_MU_BASE ( AUX_BASE + 0x40 )

#define AUX_ENABLES ( AUX_BASE + 0x04 )
#define AUX_MU_IO ( AUX_MU_BASE + 0x00 )
#define AUX_MU_IER ( AUX_MU_BASE + 0x04 )
#define AUX_MU_IIR ( AUX_MU_BASE + 0x08 )
#define AUX_MU_LCR ( AUX_MU_BASE + 0x0C )
#define AUX_MU_SCRATCH ( AUX_MU_BASE + 0x1C )
#define AUX_MU_CNTL ( AUX_MU_BASE + 0x20 )
#define AUX_MU_STAT ( AUX_MU_BASE + 0x24 )
#define AUX_MU_BAUD ( AUX_MU_BASE + 0x28 )

#define B115200 541

#define RX_PIN 15
#define TX_PIN 14
#define MU_FUNC GPIO_FUNC5

static uart_data_t* uart_data = (uart_data_t*)AUX_MU_SCRATCH;

void uart_init_baud( uint16_t baud ) {
	if ( uart_data->init ) uart_flush();

	// set gpio pins
	gpio_set_func( RX_PIN, MU_FUNC );
	gpio_set_func( TX_PIN, MU_FUNC );

	// enable uart
	// LAST READ AUX
	uint32_t val = GET32( AUX_ENABLES );
	MB_RDWR;
	val |= 1;
	// FIRST WRITE AUX
	PUT32( AUX_ENABLES, val );

	// configure
	// disable tx and rx
	// FIRST WRITE MU
	MB_WR;
	PUT32( AUX_MU_CNTL, 0 );

	// clear FIFOs
	PUT32( AUX_MU_IIR, 0b11 << 1 );

	// disable interrupts
	PUT32( AUX_MU_IER, 0 );

	// set baudrate
	PUT32( AUX_MU_BAUD, baud );

	// set to 8 bit mode
	PUT32( AUX_MU_LCR, 0b11 );

	// reenable tx and rx
	PUT32( AUX_MU_CNTL, 0b11 );

	uart_data->init = 1;
}

void uart_init( void ) {
	uart_init_baud( B115200 );
}

void uart_write( uint8_t data ) {
	while ( !uart_can_write() )
		;

	// FIRST WRITE
	MB_WR;
	PUT32( AUX_MU_IO, data );
}

uint8_t uart_read( void ) {
	while ( !uart_can_read() )
		;

	// LAST READ
	uint8_t val = GET32( AUX_MU_IO ) & 0xff;
	MB_RD;
	return val;
}

unsigned uart_can_write( void ) {
	// LAST READ
	uint32_t val = ( GET32( AUX_MU_STAT ) >> 1 ) & 1;
	MB_RD;
	return val;
}

unsigned uart_can_read( void ) {
	// LAST READ
	uint32_t val = GET32( AUX_MU_STAT ) & 1;
	MB_RD;
	return val;
}

void uart_flush( void ) {
	// LAST READ
	while ( !( ( GET32( AUX_MU_STAT ) >> 9 ) & 1 ) )
		;
	MB_RD;
}

unsigned uart_is_initialized( void ) {
	return uart_data->init;
}

uart_data_t* get_uart_data( void ) {
	return uart_data;
}
