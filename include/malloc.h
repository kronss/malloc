#ifndef MALLOC_H
# define MALLOC_H

#include <stdio.h> /*printf remove*/


void                    *malloc(size_t size);
void                    *realloc(void *ptr, size_t size);
void					free(void *ptr);
void 					show_alloc_mem();

#endif /*MALLOC_H*/





/*
 * TODO:
 * check in new_zone size requests in all zones
 *
 *
 */
