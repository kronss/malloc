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
//int main(int ac, char **av)
//{
//    char *ptr0 = NULL;
//    char *ptr1 = NULL;
//    char *ptr2 = NULL;
//    char *ptr3 = NULL;
//    char *ptr4 = NULL;
//    char *ptr5 = NULL;
//    char *ptr6 = NULL;
//
//
////    printf("sizeof struct block_s == %zu\n", sizeof(struct block_s));
////    printf("sizeof struct zone_s == %zu\n", sizeof(struct zone_s));
//
////    for (int i = 0; i < 1000000; ++i) {
////    show_alloc_mem();
//    printf("---------------------------------------------------------\n");
//
//    	ptr0 = malloc(1*M);
//		ptr1 = malloc(2*M);
//		ptr2 = malloc(3*M);
//		ptr3 = malloc(4*M);
//        ptr4 = malloc(5*M);
//        ptr5 = malloc(6*M);
//        ptr6 = malloc(7*M);
//
//
//        show_alloc_mem();
//
//    printf("---------------------------------------------------------\n");
//////
//////	  printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr0);
//////	  printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr1);
//    	free(ptr3);
////    	free(ptr3);
//////    	free(ptr2);
//        show_alloc_mem();
//
//        ptr3 = malloc(4*M);
//        show_alloc_mem();
//
//////
////printf("---------------------------------------------------------\n");
////    	ptr0 = malloc(1);
////
////	ptr1 = realloc(ptr2, 15000);
////    show_alloc_mem();
//
////    free(NULL + 0x1);
////    ptr = malloc(3111);
////
////    show_alloc_mem();
//
////    printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr);
////    ptr = malloc(4);
////    printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr);
//
//
//
//    return (0);
//}
//
//int main ()
//{
//malloc (1024);
//malloc (1024 * 32);
//malloc (1024 * 1024);
//malloc (1024 * 1024 * 16);
//malloc (1024 * 1024 * 128);
//show_alloc_mem ();
//return (0);
//}

int main()
{
int i;
char *addr;

i = 0;
//addr = (char*)malloc(1024);
while (i < 1024)
{
addr = (char*)malloc(1024);
addr[0] = 42;
free(addr);
i++;
}
return (0);
}



