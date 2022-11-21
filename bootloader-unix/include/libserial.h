#ifndef _LIBSERIAL_H_
#define _LIBSERIAL_H_

// returns fd of configured serial device specified by devname
int get_usb( const char* devname );

// returns fd of configured serial device specified by find_ttyusb
int get_ttyusb( void );

// returns fd of configured serial device specified by find_ttyusb_last
int get_ttyusb_last( void );

// returns fd of configured serial device specified by find_ttyusb_first
int get_ttyusb_first( void );

#endif
