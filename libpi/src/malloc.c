#include "rpi.h"
#include "malloc.h"

#include "sbrk.h"

#define HDR_SIZE sizeof( hdr_t )

typedef struct hdr {
	size_t num_chunks : 61; // size in chunks of 8
	size_t prev_free : 1;
	size_t free : 1;
	size_t last : 1;
} hdr_t;

_Static_assert( HDR_SIZE == 8, "header struct wrong size" );

static hdr_t* first_block = NULL;

#define alloc_new_block( nchunks, pfree )                                                                              \
	( {                                                                                                            \
		hdr_t* block = sbrk( ( nchunks + 1 ) * 8 );                                                            \
		if ( !block ) return NULL;                                                                             \
		block->num_chunks = nchunks;                                                                           \
		block->last = 1;                                                                                       \
		block->prev_free = pfree;                                                                              \
		block;                                                                                                 \
	} )

// essentially rounds up to nearest multiple of 8 then divides by 8
static inline size_t calc_chunks( size_t bytes ) {
	return ( bytes + 0b111 ) >> 3;
}

static inline hdr_t* get_next_header( hdr_t* h ) {
	return h + 1 + h->num_chunks;
}

static inline hdr_t* get_prev_header( hdr_t* h ) {
	return *( (hdr_t**)h - 1 );
}

static inline int block_good( hdr_t* h, size_t needed_chunks ) {
	return h->num_chunks >= needed_chunks && h->free;
}

static inline void set_block_free( hdr_t* h ) {
	hdr_t** prev_pointer = (hdr_t**)h + h->num_chunks;
	*prev_pointer = h;
	h->free = 1;
	if ( !h->last ) get_next_header( h )->prev_free = 1;
}

static inline void set_block_unfree( hdr_t* h ) {
	h->free = 0;
	if ( !h->last ) get_next_header( h )->prev_free = 0;
}

void* malloc( size_t bytes ) {
	if ( bytes == 0 ) return NULL;

	size_t needed_chunks = calc_chunks( bytes );

	if ( first_block == NULL ) {
		first_block = alloc_new_block( needed_chunks, 0 );
		first_block->free = 0;
		return first_block + 1;
	}

	hdr_t* cur_block = first_block;
	while ( !block_good( cur_block, needed_chunks ) && cur_block->last != 1 )
		cur_block = get_next_header( cur_block );

	hdr_t* chosen_block;
	if ( block_good( cur_block, needed_chunks ) ) {
		chosen_block = cur_block;
	}
	else {
		chosen_block = alloc_new_block( needed_chunks, cur_block->free );
		cur_block->last = 0;
	}

	if ( chosen_block->num_chunks <= needed_chunks + 1 ) {
		set_block_unfree( chosen_block );
		return chosen_block + 1;
	}

	size_t original_num_chunks = chosen_block->num_chunks;
	chosen_block->num_chunks = needed_chunks;

	hdr_t* split = get_next_header( chosen_block );
	split->num_chunks = original_num_chunks - ( needed_chunks + 1 );
	set_block_free( split );
	split->last = chosen_block->last;

	chosen_block->last = 0;
	set_block_unfree( chosen_block );

	return chosen_block + 1;
}

void* calloc( size_t bytes ) {
	uint64_t* ptr = malloc( bytes );
	if ( !ptr ) return NULL;

	size_t needed_chunks = calc_chunks( bytes );

	for ( size_t i = 0; i < needed_chunks; i++ ) ptr[i] = 0;

	return ptr;
}

void free( void* ptr ) {
	if ( !ptr ) return;

	hdr_t* block = ptr;
	block--;

	size_t total_num_chunks = block->num_chunks;
	int last = block->last;

	if ( !last ) {
		hdr_t* next = get_next_header( block );
		if ( next->free ) {
			total_num_chunks += next->num_chunks + 1;
			last = next->last;
		}
	}

	if ( block->prev_free ) {
		hdr_t* prev = get_prev_header( block );
		total_num_chunks += prev->num_chunks + 1;
		block = prev;
	}

	block->num_chunks = total_num_chunks;
	block->last = last;
	set_block_free( block );
}
