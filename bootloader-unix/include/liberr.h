#ifndef _LIBERR_H_
#define _LIBERR_H_

#include <stdio.h>
#include <stdlib.h>

#define die( msg... )                                                                                                  \
	do {                                                                                                           \
		fprintf( stderr, msg );                                                                                \
		exit( 1 );                                                                                             \
	} while ( 0 )

#define panic( msg... )                                                                                                \
	do {                                                                                                           \
		fprintf( stderr, "%s:%d:PANIC:", __FILE__, __LINE__ );                                                 \
		die( msg );                                                                                            \
	} while ( 0 )

#endif
