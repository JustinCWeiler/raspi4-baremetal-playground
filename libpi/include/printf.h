#ifndef _PRINTF_H_
#define _PRINTF_H_

//#include "rpi.h"

size_t puts( const char* src );
size_t dputs( void ( *write_fun )( uint8_t, void* ), void* aux, const char* src );

size_t printf( const char* fmt, ... ) __attribute__( ( format( printf, 1, 2 ) ) );
size_t sprintf( char* buf, const char* fmt, ... ) __attribute__( ( format( printf, 2, 3 ) ) );
size_t dprintf( void ( *write_fun )( uint8_t ), const char* fmt, ... ) __attribute__( ( format( printf, 2, 3 ) ) );
size_t snprintf( char* buf, size_t size, const char* fmt, ... ) __attribute__( ( format( printf, 3, 4 ) ) );

#endif
