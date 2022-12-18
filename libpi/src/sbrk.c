#include "rpi.h"
#include "sbrk.h"

char* heap_start;
char* heap_ptr;
char* heap_end;

void* sbrk( size_t bytes ) {
	if ( heap_ptr + bytes > heap_end ) return NULL;

	void* ret = heap_ptr;
	heap_ptr += bytes;
	return ret;
}

void reset_heap( void* start, void* end ) {
	heap_start = start;
	heap_ptr = start;
	heap_end = end;
}
