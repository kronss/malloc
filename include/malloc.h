#ifndef MALLOC_H
# define MALLOC_H

#include <sys/user.h> /* PAGE_SIZE */


#define ALIGN_META_INFO(x) (((((x) -1) >> 2) << 2) + 4)

#define BLOCK_SIZE sizeof(struct s_block)



/* block struct */
struct s_block {
	size_t           size;
	s_block         *next;
	s_block         *prev;
	int              free;
	void            *ptr;
	char             data[1];                /* A pointer to the allocated block */
};


enum {
	TINY = 1,
	SMALL = 2,
	LARGE = 3
};


enum {
	TINY_ZONE  = PAGE_SIZE < 4096 ? 4096 : PAGE_SIZE,
	SMALL_ZONE = TINY_ZONE * 100,
	LARGE_ZONE = SMALL_ZONE + 1   //TODO: rework!
};

enum {


};






void                    *malloc(size_t size);





#endif /*MALLOC_H*/
