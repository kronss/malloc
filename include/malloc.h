#ifndef MALLOC_H
# define MALLOC_H

#include <sys/user.h> /* PAGE_SIZE */


#define ALIGN_META_INFO(x) (((((x) -1) >> 2) << 2) + 4)

#define BLOCK_SIZE sizeof(struct s_block)





void                    *malloc(size_t size);





#endif /*MALLOC_H*/
