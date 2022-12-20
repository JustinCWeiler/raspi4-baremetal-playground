#include "rpi.h"
#include "random.h"

// shamelessly stolen from circle library

#define RAND_BASE ( PERIPHERAL_BASE + 0x00104000 )
#define RAND_CTRL ( RAND_BASE + 0x00 )
#define RAND_TOTAL_BIT_COUNT ( RAND_BASE + 0X0C )
#define RAND_TOTAL_BIT_COUNT_THRESHOLD ( RAND_BASE + 0x10 )
#define RAND_FIFO_DATA ( RAND_BASE + 0x20 )
#define RAND_FIFO_COUNT ( RAND_BASE + 0x24 )

#define RAND_CTRL_RBGEN_MASK 0x00001FFF
#define RAND_CTRL_DIV_SHIFT 13

#define RAND_FIFO_COUNT_THRESHOLD_SHIFT 8
#define RAND_FIFO_COUNT_MASK 0x000000FF

void rand_init( void ) {
	// set first few generated numbers to be discarded
	// FIRST WRITE
	MB_WR;
	PUT32( RAND_TOTAL_BIT_COUNT_THRESHOLD, 0x40000 );

	// min fifo count to generate interrupt
	PUT32( RAND_FIFO_COUNT, 2 << RAND_FIFO_COUNT_THRESHOLD_SHIFT );

	// enable rng, 1MHz sample rate
	PUT32( RAND_CTRL, ( 3 << RAND_CTRL_DIV_SHIFT ) | RAND_CTRL_RBGEN_MASK );

	// LAST READ
	while ( GET32( RAND_TOTAL_BIT_COUNT ) <= 16 )
		;
	MB_RD;
}

uint32_t rand32( void ) {
	while ( ( GET32( RAND_FIFO_COUNT ) & RAND_FIFO_COUNT_MASK ) == 0 )
		;

	// LAST READ
	uint32_t ret = GET32( RAND_FIFO_DATA );
	MB_RD;

	return ret;
}
