#ifndef _SBRK_H_
#define _SBRK_H_

//#include "rpi.h"

void* sbrk( size_t bytes );

// WARNING: INVALIDATES HEAP
void reset_heap( void* start, void* end );

#endif
