#ifndef _LIBERR_H_
#define _LIBERR_H_

#include <stdlib.h>
#include <stdio.h>

#define die(msg...) do {	\
	fprintf(stderr, msg);	\
	exit(1);		\
} while(0)

#endif
