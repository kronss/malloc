#ifndef MALLOC_H
# define MALLOC_H

#include <sys/user.h> /* PAGE_SIZE */
#include <sys/mman.h>

#define ALIGN_PAGE_SIZE(len_) (len_) = ((PAGE_SIZE - 1) & (len_)) ? (((len_) + PAGE_SIZE) & ~(PAGE_SIZE - 1)) : (len_)


#define ALIGN_META_INFO(x) (x) = (((((x) - 1) >> 2) << 2) + 4)  //TODO: re work

#define BLOCK_SIZE sizeof(struct s_block)

enum zone_type_e {
	TINY = 1,
	SMALL = 2,
	LARGE = 3,

	MAX_ZONE_TYPE
};

/* block struct */
struct block_s {
	size_t           size;           /* size + meta_info */
	struct block_s  *next;
	struct block_s  *prev;
	int              free;
	void            *ptr;
	char             data[1];                /* A pointer to the allocated block */
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
	struct block_s		md_block_head[];
};


struct malloc_meneger_s {
    struct zone_s *zone_heads[MAX_ZONE_TYPE];
//    size_t malloc_call[MAX_ZONE_TYPE];
};










enum zone_size_e {
	TINY_ZONE  = PAGE_SIZE < 4096 ? 4096 : PAGE_SIZE,
	SMALL_ZONE = TINY_ZONE * 100,
	LARGE_ZONE = SMALL_ZONE + 1,   //TODO: rework!
};

enum zone_treshold_e{
    TINY_TRESHHOLD = TINY_ZONE / 100,
    SMALL_TRESHHOLD = SMALL_ZONE / 100,
    LARGE_TRESHHOLD = LARGE_ZONE / 100,

};





void                    *malloc(size_t size);





#endif /*MALLOC_H*/
