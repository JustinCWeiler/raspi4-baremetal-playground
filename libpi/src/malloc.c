#include "rpi.h"
#include "malloc.h"

#include "printf.h"
#include "sbrk.h"

#define roundup( x, n ) ( ( ( x ) + ( (n)-1 ) ) & ( ~( (n)-1 ) ) )

#define HDR_SIZE sizeof( hdr_t )

typedef struct hdr {
	size_t size; // excluding header
	struct hdr* next;
	int free;
} hdr_t;

_Static_assert( HDR_SIZE % 8 == 0, "header struct misaligned size" );

static hdr_t* first_block = NULL;

#define alloc_new_block( bytes )                                                                                       \
	( {                                                                                                            \
		hdr_t* block = sbrk( bytes + HDR_SIZE );                                                               \
		if ( !block ) return NULL;                                                                             \
		block->size = bytes;                                                                                   \
		block->free = 1;                                                                                       \
		block->next = NULL;                                                                                    \
		block;                                                                                                 \
	} )

static inline int block_good( hdr_t* h, size_t size ) {
	return h->size >= size && h->free;
}

void* malloc( size_t bytes ) {
	if ( bytes == 0 ) return NULL;

	bytes = roundup( bytes, 8 );

	if ( first_block == NULL ) {
		first_block = alloc_new_block( bytes );
		first_block->free = 0;
		return first_block + 1;
	}

	hdr_t* cur_block = first_block;
	while ( !block_good( cur_block, bytes ) && cur_block->next != NULL ) cur_block = cur_block->next;

	hdr_t* chosen_block;
	if ( block_good( cur_block, bytes ) ) {
		chosen_block = cur_block;
	}
	else {
		chosen_block = alloc_new_block( bytes );
		cur_block->next = chosen_block;
	}

	if ( chosen_block->size <= bytes + HDR_SIZE ) {
		chosen_block->free = 0;
		return chosen_block + 1;
	}

	uintptr_t usplit = (uintptr_t)chosen_block;
	usplit += HDR_SIZE;
	usplit += bytes;
	hdr_t* split = (hdr_t*)usplit;

	split->size = chosen_block->size - bytes - HDR_SIZE;
	split->next = chosen_block->next;
	split->free = 1;

	chosen_block->size = bytes;
	chosen_block->next = split;
	chosen_block->free = 0;

	return chosen_block + 1;
}

void* calloc( size_t bytes ) {
	char* ptr = malloc( bytes );
	if ( !ptr ) return NULL;

	for ( size_t i = 0; i < bytes; i++ ) ptr[i] = 0;

	return ptr;
}

void free( void* ptr ) {
	hdr_t* block = ptr;
	block--;
	block->free = 1;
}
