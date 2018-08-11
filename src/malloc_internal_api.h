#ifndef MALLOC_INTERNAL_API_H
# define MALLOC_INTERNAL_API_H

#include "malloc.h"
#include "libft.h"

#include <sys/user.h> /* PAGE_SIZE */
#include <sys/mman.h>
#include <pthread.h>
#include <stdint.h> /*uint8_t*/
//#include <unistd.h> /*getpagesize*/
//#include <stddef.h> /*offsetof*/
#include <unistd.h>


//#define ALIGN_PAGE_SIZE(len_) (len_) = ((PAGE_SIZE - 1) & (len_)) ? (((len_) + PAGE_SIZE) & ~(PAGE_SIZE - 1)) : (len_)
//
//#define METABLOCK_SIZE sizeof(struct s_block)
//
//#define ALIGN_WORD(x) (((((x) - 1) >> 2) << 2) + 4)  //TODO: re work
//#define ALIGN_META_INFO(x) (x) = ALIGN_WORD((x) + sizeof(struct block_s))  //TODO: re work


enum zone_type_e {
	MIN_ZONE_TYPE,
	TINY = MIN_ZONE_TYPE,
	SMALL,
	LARGE,

	MAX_ZONE_TYPE
};

enum insert_type_e {
    FRESH_ZONE,
    USED_ZONE,
};

/* block struct */
struct block_s {
	size_t           alloc_size;           /* size + meta_info */
	int              free     :1,
	                 reserved :31;
	struct block_s  *next;
	struct block_s  *prev;

	uint8_t          data[];        /* A pointer to the user data. Should be last */
};

/*
 *
 */
struct			zone_s
{
	size_t				origin_size;
	size_t				space_left;
	struct zone_s		*next;
	struct zone_s		*prev;
	struct block_s		md_block_head[]; /* A pointer to the head blocks. Should be last */
};


struct malloc_meneger_s {
    struct zone_s *zone_heads[MAX_ZONE_TYPE];
//    size_t zone_size[MAX_ZONE_TYPE - 1];
};




#define ALIGN_PAGE_SIZE(len_) (len_) = ((PAGE_SIZE - 1) & (len_)) ? (((len_) + PAGE_SIZE) & ~(PAGE_SIZE - 1)) : (len_)
#define METABLOCK_SIZE sizeof(struct block_s)
#define ALIGN_WORD(x) (((((x) - 1) >> 2) << 2) + 4)  //TODO: re work
#define ALIGN_META_INFO(x) (x) = ALIGN_WORD((x) + sizeof(struct block_s))  //TODO: re work






enum zone_size_e {
	TINY_ZONE  = PAGE_SIZE * 1  ,
	SMALL_ZONE = PAGE_SIZE * 1500,
	LARGE_ZONE = PAGE_SIZE * 30,
};

//enum zone_size_e {
//	TINY_ZONE  = 4096 * 1,
//	SMALL_ZONE = 4096 * 160,
//	LARGE_ZONE = 4096 * 1000,
//};

enum zone_treshold_e{
    TINY_TRESHHOLD = TINY_ZONE / 100,
    SMALL_TRESHHOLD = SMALL_ZONE / 100,
    LARGE_TRESHHOLD = LARGE_ZONE / 100,

};


void free_and_return_block_to_pull(struct zone_s *zone_ptr, struct block_s *block_ptr);
void *get_ptr_to_md(void *ptr);
int check_block_ptr(struct zone_s *zone_ptr, struct block_s *block_ptr);
int validate_md(struct zone_s **zone_ptr, struct block_s **block_ptr);
void *get_zone_head(size_t size);
void *get_ptr(size_t size);
void remove_cur_zone_to_pull(struct zone_s *zone_ptr);
int is_all_blocks_free(struct zone_s *zone_ptr);
void free_defragment_unmap(struct zone_s *zone_ptr, struct block_s *block_ptr);









extern struct malloc_meneger_s malloc_meneger_g;
extern pthread_mutex_t mutex_malloc;

#endif /*MALLOC_INTERNAL_API_H*/
