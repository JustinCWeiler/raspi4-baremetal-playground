#ifndef _MALLOC_H_
#define _MALLOC_H_

//#include "rpi.h"

void* malloc( size_t bytes );
void* calloc( size_t bytes );

void free( void* ptr );

#endif
