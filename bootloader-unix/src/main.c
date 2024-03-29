#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "bootdefs.h"
#include "libcrc.h"
#include "liberr.h"
#include "libfile.h"
#include "libserial.h"

static uint8_t read_byte( int fd ) {
	uint8_t data;
	int ret;
	if ( ( ret = read( fd, &data, 1 ) ) == 1 ) return data;

	if ( ret < 0 ) panic( "Error reading serial: %s\n", strerror( errno ) );
	else
		panic( "Connection to serial closed\n" );
}

static void write_byte( int fd, uint8_t b ) {
	if ( write( fd, &b, 1 ) < 1 ) panic( "Error writing serial: %s\n", strerror( errno ) );
}

static void print_code( uint8_t code ) {
	switch ( code ) {
		case SUCCESS:
			fprintf( stderr, "SUCCESS" );
			break;
		case GET_INFO:
			fprintf( stderr, "GET_INFO" );
			break;
		case PUT_INFO:
			fprintf( stderr, "PUT_INFO" );
			break;
		case GET_CODE:
			fprintf( stderr, "GET_CODE" );
			break;
		case PUT_CODE:
			fprintf( stderr, "PUT_CODE" );
			break;
		default:
			fprintf( stderr, "0x%02x", code );
			break;
	}
}

static void print_mismatch( int fd ) {
	uint8_t expect = read_byte( fd );
	uint8_t actual = read_byte( fd );

	fprintf( stderr, "RPi4 expected " );
	print_code( expect );
	fprintf( stderr, " but received " );
	print_code( actual );
	fprintf( stderr, "\n" );
}

int main( int argc, const char** argv ) {
	int tty_fd;
	uint8_t* prog;
	uint32_t nbytes;

top:

	if ( argc < 2 ) die( "Usage: %s [-last|-first|/dev/ttySX] <program.bin>\n", argv[0] );
	else if ( argc < 3 ) {
		nbytes = read_file( argv[1], &prog );
		tty_fd = get_ttyusb();
	}
	else if ( argc < 4 ) {
		nbytes = read_file( argv[2], &prog );
		if ( strcmp( "-last", argv[1] ) == 0 ) tty_fd = get_ttyusb_last();
		else if ( strcmp( "-first", argv[1] ) == 0 )
			tty_fd = get_ttyusb_first();
		else
			tty_fd = get_usb( argv[1] );
	}
	else
		die( "Error: too many arguments" );

	uint32_t crc = crc32( prog, nbytes );

	// read GET_INFO
	uint8_t recv;
	while ( ( recv = read_byte( tty_fd ) ) != GET_INFO ) {
		if ( recv == CODE_MISMATCH ) print_mismatch( tty_fd );
		fprintf( stderr, "Expected GET_INFO, received " );
		print_code( recv );
		fprintf( stderr, ". Restarting process...\n" );

		goto top;
	}

	// write PUT_INFO
	write_byte( tty_fd, PUT_INFO );

	// write nbytes
	for ( int i = 0; i < 32; i += 8 ) write_byte( tty_fd, nbytes >> i );

	// write crc
	for ( int i = 0; i < 32; i += 8 ) write_byte( tty_fd, crc >> i );

	// drain buffer
	while ( ( recv = read_byte( tty_fd ) ) == GET_INFO )
		;

	// read GET_CODE
	if ( recv != GET_CODE ) {
		if ( recv == CODE_MISMATCH ) print_mismatch( tty_fd );
		fprintf( stderr, "Expected GET_CODE, received " );
		print_code( recv );
		fprintf( stderr, ". Restarting process...\n" );

		goto top;
	}

	// read crc
	for ( int i = 0; i < 32; i += 8 ) {
		recv = read_byte( tty_fd );
		if ( recv != ( ( crc >> i ) & 0xff ) )
			panic( "Expected crc byte 0x%02x and got 0x%02x\n", ( crc >> i ) & 0xff, recv );
	}

	// write PUT_CODE
	write_byte( tty_fd, PUT_CODE );

	// write program bytes
	for ( uint32_t i = 0; i < nbytes; i++ ) {
		write_byte( tty_fd, prog[i] );
	}

	// read SUCCESS
	recv = read_byte( tty_fd );
	uint32_t crc_bad;
	switch ( recv ) {
		case SUCCESS:
			break;
		case CRC_FAIL:
			crc_bad = read_byte( tty_fd );
			crc_bad |= read_byte( tty_fd ) << 8;
			crc_bad |= read_byte( tty_fd ) << 16;
			crc_bad |= read_byte( tty_fd ) << 24;
			panic( "CRC Mismatch: expected 0x%08x got 0x%08x\n", crc, crc_bad );
			break;
		default:
			panic( "Expected SUCCESS, got 0x%02x\n", recv );
	}

	printf( "Successfully booted!\n" );
	printf( "Echoing output from Pi\n\n" );

	int eots_read = 0;
	while ( 1 ) {
		uint8_t read = read_byte( tty_fd );
		if ( read == 0x04 ) eots_read++;
		else {
			// could use for loop but compiler might not understand
			// that eots_read < 4
			if ( eots_read > 0 ) putchar( 0x04 );
			if ( eots_read > 1 ) putchar( 0x04 );
			if ( eots_read > 2 ) putchar( 0x04 );
			eots_read = 0;
			putchar( read );
			fflush( stdout );
		}

		if ( eots_read >= 4 ) {
			printf( "\nPi sent end of program signal! Exiting now...\n" );
			exit( 0 );
		}
	}
}
