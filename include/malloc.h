#ifndef MALLOC_H
# define MALLOC_H


#define align4(x) (((((x) -1) >> 2) << 2) + 4)

#define BLOCK_SIZE sizeof(struct s_block)


void                    *malloc(size_t size);





#endif /*MALLOC_H*/
