#ifndef _LIBTTY_H_
#define _LIBTTY_H_

// returns "-1"-terminated list of ttyusb's
// XXX THIS FUNCTION ONLY WORKS ON WSL1 -- WILL NEED TO BE CHANGED ON OTHER SYSTEMS
int* find_ttyusb_list(void);

// returns string corresponding to /dev/ttyS device
// if there are multiple, errors
char* find_ttyusb(void);

// returns the last modified (usually last inserted) /dev/ttyS device
char* find_ttyusb_last(void);

// returns the first modified (usually first inserted) /dev/ttyS device
char* find_ttyusb_first(void);

#endif
