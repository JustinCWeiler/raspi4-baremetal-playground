#ifndef _LIBFILE_H_
#define _LIBFILE_H_

#include <stddef.h>
#include <stdint.h>

// allocates buffer in buf that is the size of filename and reads the file into it
// returns the number of bytes read
uint32_t read_file( const char* filename, uint8_t** buf );

#endif
