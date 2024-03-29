#include "rpi.h"
#include "printf.h"

#include "stdarg.h"
#include "uart.h"

typedef void ( *write_fun_t )( uint8_t, void* );

size_t puts( const char* src ) {
	size_t count = 0;

	while ( *src ) {
		uart_write( *src++ );
		count++;
	}

	uart_write( '\n' );
	count++;

	return count;
}

size_t dputs( write_fun_t write_fun, void* aux, const char* src ) {
	size_t count = 0;

	while ( *src ) {
		write_fun( *src++, aux );
		count++;
	}

	write_fun( '\n', aux );
	count++;

	return count;
}

// ---------- Complicated stuff :) ----------

static inline int mult_overflow( uint64_t a, uint64_t b ) {
	return a * b < a;
}

// assumes radix <= 36
// which is assured by anyone who edits this file (so basically me)
// :)
static inline size_t
write_radix( write_fun_t write_fun, void* aux, uint64_t val, size_t min_digits, size_t radix, int signd, int capital ) {
	size_t count = 0;
	if ( signd && val & ( 1lu << 63 ) ) {
		write_fun( '-', aux );
		count++;
		val = ~val + 1;
	}

	uint64_t divisor = 1;
	for ( size_t i = 1; i < min_digits && !mult_overflow( divisor, radix ); i++ ) {
		divisor *= radix;
	}

	while ( val / divisor >= radix && !mult_overflow( divisor, radix ) ) {
		divisor *= radix;
	}

	while ( 1 ) {
		uint64_t digit = val / divisor;
		char c;
		if ( digit < 0xa ) c = digit + '0';
		else
			c = digit - 0xa + ( capital ? 'A' : 'a' );

		write_fun( c, aux );
		count++;

		if ( divisor == 1 ) break;

		val %= divisor;
		divisor /= radix;
	}

	return count;
}

static inline size_t handle_num( write_fun_t write_fun, void* aux, const char** fmt_ptr, va_list* va ) {
	const char* fmt = *fmt_ptr;

	size_t min_digits = 0;
	if ( *fmt == '0' ) {
		char c = *++fmt;
		while ( '0' <= c && c <= '9' ) {
			min_digits *= 10;
			min_digits += c - '0';
			c = *++fmt;
		}
	}

	uint64_t val;
	if ( *fmt == 'l' ) {
		fmt++;
		if ( *fmt == 'd' ) val = va_arg( *va, int64_t );
		else
			val = va_arg( *va, uint64_t );
	}
	else {
		if ( *fmt == 'd' )
			// cast to sign extend
			val = (int64_t)va_arg( *va, int32_t );
		else
			val = va_arg( *va, uint32_t );
	}

	size_t radix;
	int signd, capital;
	switch ( *fmt ) {
		case 'd':
			radix = 10;
			signd = 1;
			capital = 0;
			break;
		case 'u':
			radix = 10;
			signd = 0;
			capital = 0;
			break;
		case 'o':
			radix = 8;
			signd = 0;
			capital = 0;
			break;
		case 'x':
			radix = 16;
			signd = 0;
			capital = 0;
			break;
		case 'X':
			radix = 16;
			signd = 0;
			capital = 1;
			break;
		default:
			return 0;
	}

	*fmt_ptr = fmt;
	return write_radix( write_fun, aux, val, min_digits, radix, signd, capital );
}

static inline size_t handle_str( write_fun_t write_fun, void* aux, const char* src ) {
	size_t count = 0;

	while ( *src ) {
		write_fun( *src++, aux );
		count++;
	}

	return count;
}

// write_fun is a func ptr that takes the character to write and a ptr to auxilliary data
// originally i was planning on using nested functions to keep track of aux data but
// that proved clunky and also not portable (although portability isnt really a priority)
// the new solution is also kinda clunky but now in an implementation way rather than
// a "gotta deal with the compiler" way
static size_t vdprintf( write_fun_t write_fun, void* aux, const char* fmt, va_list va ) {
	size_t count = 0;

	while ( *fmt ) {
		if ( *fmt == '%' ) {
			fmt++;
			switch ( *fmt ) {
				case '0':
				case 'l':
				case 'd':
				case 'u':
				case 'o':
				case 'x':
				case 'X':
					count += handle_num( write_fun, aux, &fmt, &va );
					fmt++;
					break;
				case 'c':
					// we say int here but its casted down to a char
					// have to say int because smth smth gcc no likey char for va_arg
					write_fun( va_arg( va, int ), aux );
					count++;
					fmt++;
					break;
				case 's':
					count += handle_str( write_fun, aux, va_arg( va, char* ) );
					fmt++;
					break;
				case 'p':
					write_fun( '0', aux );
					write_fun( 'x', aux );
					count +=
					  write_radix( write_fun, aux, va_arg( va, uintptr_t ), 0, 16, 0, 0 ) + 2;
					fmt++;
					break;
				case '%':
					write_fun( '%', aux );
					count++;
					fmt++;
					break;
				default:
					write_fun( '%', aux );
					write_fun( *fmt, aux );
					count += 2;
					fmt++;
			}
		}
		else {
			write_fun( *fmt++, aux );
			count++;
		}
	}

	return count;
}

// ---------- printf ----------

// needed because uart_write is an external function so its location
// is wonky
static void printf_write( uint8_t c, void* aux ) {
	uart_write( c );
	(void)aux;
}

size_t printf( const char* fmt, ... ) {
	va_list va;
	va_start( va, fmt );

	size_t count = vdprintf( printf_write, NULL, fmt, va );

	va_end( va );

	return count;
}

// ---------- sprintf ----------

struct sprintf_data {
	char* buf;
	size_t pos;
};

static void sprintf_write( uint8_t c, void* aux ) {
	struct sprintf_data* data = aux;

	data->buf[data->pos++] = c;
}

size_t sprintf( char* buf, const char* fmt, ... ) {
	va_list va;
	va_start( va, fmt );

	struct sprintf_data data = { .buf = buf, .pos = 0 };
	size_t count = vdprintf( sprintf_write, &data, fmt, va );

	va_end( va );

	return count;
}

// ---------- dprintf ----------

size_t dprintf( void ( *write_fun )( uint8_t ), const char* fmt, ... ) {
	va_list va;
	va_start( va, fmt );

	size_t count = vdprintf( ( write_fun_t )(void*)write_fun, NULL, fmt, va );

	va_end( va );

	return count;
}

// ---------- snprintf ----------

struct snprintf_data {
	char* buf;
	size_t n;
	size_t pos;
};

static void snprintf_write( uint8_t c, void* aux ) {
	struct snprintf_data* data = aux;

	if ( data->pos >= data->n - 1 ) data->buf[data->n - 1] = 0;
	else
		data->buf[data->pos++] = c;
}

size_t snprintf( char* buf, size_t size, const char* fmt, ... ) {
	va_list va;
	va_start( va, fmt );

	struct snprintf_data data = { .buf = buf, .n = size, .pos = 0 };
	size_t count = vdprintf( snprintf_write, &data, fmt, va );

	va_end( va );

	return count;
}
