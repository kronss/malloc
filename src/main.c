#include <stdio.h>
#include <stdlib.h>

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
    ptr = malloc(-1);

    printf("--%p\n", ptr);



    return (0);
}
