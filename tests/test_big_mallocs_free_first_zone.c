#include <stdio.h>
#include "malloc.h"
#include <string.h>
#define M (1024 * 1024)

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


//
int main(int ac, char **av)
{
    char *ptr0 = NULL;
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
//    char *ptr4 = NULL;


//    printf("sizeof struct block_s == %zu\n", sizeof(struct block_s));
//    printf("sizeof struct zone_s == %zu\n", sizeof(struct zone_s));

//    for (int i = 0; i < 1000000; ++i) {
//    show_alloc_mem();
    printf("---------------------------------------------------------\n");

    	ptr0 = malloc(M);
		ptr1 = malloc(2*M);
		ptr2 = malloc(3*M);
		ptr3 = malloc(4*M);

    show_alloc_mem();

    printf("---------------------------------------------------------\n");
////
////	  printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr0);
////	  printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr1);
    	free(ptr0);
//    	free(ptr3);
////    	free(ptr2);
        show_alloc_mem();
////
//printf("---------------------------------------------------------\n");
//    	ptr0 = malloc(1);
//
//	ptr1 = realloc(ptr2, 15000);
//    show_alloc_mem();

//    free(NULL + 0x1);
//    ptr = malloc(3111);
//
//    show_alloc_mem();

//    printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr);
//    ptr = malloc(4);
//    printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr);



    return (0);
}
//
//
//int main()
//{
//char *addr1;
//char *addr2;
//char *addr3;
//
//addr1 = (char*)malloc(16*M);
//strcpy(addr1, "Bonjour\n");
//printf("%s\n", addr1);
//addr2 = (char*)malloc(16*M);
//addr3 = (char*)realloc(addr1, 128*M);
//addr3[127*M] = 42;
//printf("%s\n", addr1);
//return (0);
//}


