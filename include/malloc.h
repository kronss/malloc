#ifndef MALLOC_H
# define MALLOC_H

#include <stddef.h> /*offsetof*/
#include <stdio.h> /*printf remove*/


void                    *malloc(size_t size);
//void                    *realloc(size_t size);
void					free(void *ptr);
void 					show_alloc_mem();

#endif /*MALLOC_H*/
