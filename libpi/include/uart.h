#ifndef _UART_H_
#define _UART_H_

//#include "rpi.h"

typedef struct {
	uint32_t init : 1;
	uint32_t SBZ : 7;

	uint32_t UNDEFINED : 24;
} uart_data_t;
_Static_assert( sizeof( uart_data_t ) == 4, "uart_data_t is not 32 bits" );

// initialize uart to baudrate `baud`
// note that `baud` isn't the baudrate, but rather the value to set the buad register to
void uart_init_baud( uint16_t baud );
// initialize uart to default baudrate (115200)
void uart_init( void );

// write byte to uart
void uart_write( uint8_t data );
// read byte from uart
uint8_t uart_read( void );

// check whether we can write
unsigned uart_can_write( void );
// check whether we can read
unsigned uart_can_read( void );

// flush the data in the uart
void uart_flush( void );

// check whether the uart has been initialized
unsigned uart_is_initialized( void );

// return the uart_data_t struct
uart_data_t* get_uart_data( void );

#endif
