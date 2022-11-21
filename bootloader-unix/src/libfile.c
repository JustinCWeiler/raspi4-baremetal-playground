#include "libfile.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "liberr.h"

uint32_t read_file( const char* filename, uint8_t** buf ) {
	int fd = open( filename, O_RDONLY );
	if ( fd < 0 ) panic( "Error opening file %s: %s\n", filename, strerror( errno ) );

	struct stat s;
	fstat( fd, &s );

	ssize_t size = s.st_size;

	*buf = malloc( size );

	if ( read( fd, *buf, size ) != size ) panic( "Unexpected number of bytes read\n" );

	return size;
}
