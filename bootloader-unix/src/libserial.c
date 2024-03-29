#include "libserial.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#include "liberr.h"
#include "libtty.h"

int get_usb( const char* devname ) {
	int fd = open( devname, O_RDWR );

	if ( fd < 0 ) panic( "Error opening file: %s\n", strerror( errno ) );

	struct termios tty;

	if ( tcgetattr( fd, &tty ) < 0 ) panic( "Error getting termios attrs: %s\n", strerror( errno ) );

	cfmakeraw( &tty );
	cfsetspeed( &tty, B115200 );

	if ( tcsetattr( fd, TCSANOW, &tty ) < 0 ) panic( "Error setting termios attrs: %s\n", strerror( errno ) );

	return fd;
}

int get_ttyusb( void ) {
	return get_usb( find_ttyusb() );
}

int get_ttyusb_last( void ) {
	return get_usb( find_ttyusb_last() );
}

int get_ttyusb_first( void ) {
	return get_usb( find_ttyusb_first() );
}
