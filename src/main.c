#include <stdio.h>
#include "../include/malloc.h"

/*
 * ◦  mmap(2)
◦     munmap(2)
◦     getpagesize(3)
◦     getrlimit(2)
◦ The authorized functions within your libft (write(2) par exemple ;-) )
◦ The functions from libpthread
 */


/*
 * #include <stdlib.h>
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
3 void *reallocf(void *p, size_t size)
calloc
 */



//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



int main(int ac, char **av)
{
    char *ptr = NULL;

    printf("sizeof struct block_s == %zu\n", sizeof(struct block_s));
    printf("sizeof struct zone_s == %zu\n", sizeof(struct zone_s));

    for (int i = 0; i < 10000; ++i) {
    	ptr = malloc(8);
	}

//
//    printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr);
//    ptr = malloc(4);
//    printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr);



    return (0);
}
