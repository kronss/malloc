#include "malloc_internal_api.h"

static inline int is_prev_free(struct block_s *block_ptr)
{
    int ret_val = 0;

    if (block_ptr->prev) {
        if (block_ptr->prev->free) {
            ret_val = 1;
        }
    }

    return ret_val;
}

static inline int is_next_free(struct block_s *block_ptr)
{
    int ret_val = 0;

    if (block_ptr->next) {
        if (block_ptr->next->free) {
            ret_val = 1;
        }
    }

    return ret_val;
}

static inline void merge_two_free_blocks(struct block_s *a, struct block_s *b)
{
    if (a && b) {

        a->next = b->next;
        a->alloc_size += b->alloc_size;

        if (b->next) {
            b->next->prev = a;
        }

//      ft_memset(b, 0, sizeof(struct block_s));
    }
}

static void try_defragment(struct zone_s *zone_ptr, struct block_s *block_ptr)
{
    if (is_next_free(block_ptr)) {
        merge_two_free_blocks(block_ptr, block_ptr->next);
    }
    if (is_prev_free(block_ptr)) {
        merge_two_free_blocks(block_ptr->prev, block_ptr);
    }
}

void free_and_return_block_to_pull(struct zone_s *zone_ptr, struct block_s *block_ptr)
{
    block_ptr->free = 1;
    zone_ptr->space_left += block_ptr->alloc_size;
    try_defragment(zone_ptr, block_ptr);
}
