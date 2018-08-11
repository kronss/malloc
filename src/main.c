//#include <stdio.h>
#include "malloc.h"

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


////
//int main(int ac, char **av)
//{
//    char *ptr0 = NULL;
//    char *ptr1 = NULL;
//    char *ptr2 = NULL;
//    char *ptr3 = NULL;
//
//
////    printf("sizeof struct block_s == %zu\n", sizeof(struct block_s));
////    printf("sizeof struct zone_s == %zu\n", sizeof(struct zone_s));
//
////    for (int i = 0; i < 1000000; ++i) {
//    	ptr0 = malloc(3);
//		ptr1 = malloc(3);
//		ptr2 = malloc(3);
//		ptr3 = malloc(3);
//
//    show_alloc_mem();
//
//    	free(ptr0);
//    	free(ptr1);
//    	free(ptr2);
//        show_alloc_mem();
//
//    	free(ptr3);
//
//    show_alloc_mem();
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
//int i;
//char * addr;
//
//i = 0;
//while (i <1024)
//{
////	addr = (char*)malloc(1024);
////	addr[0] = 42;
//i ++;
//}
//
//show_alloc_mem();
//
//return (0);
//}

//int main ()
//{
//int i;
//char * addr;
//
//i = 0;
////while (i < 1024)
////{
////	addr = (char *) malloc (1024);
//////	printf("i = %i\n", i);
//////	printf("p = %p\n", addr);
////
////	addr [0] = 42;
////	free(0x01);
////	i ++;
////}
//show_alloc_mem();
//
//
//return (0);
//}


void print(char *s)
{
write(1, s, strlen(s));
}

int main()
{
char *addr;

addr = malloc(16);
show_alloc_mem();
free(NULL);
free((void *)addr + 5);
show_alloc_mem();
if (realloc((void *)addr + 5, 10) == NULL)
print("Bonjour\n");
show_alloc_mem();
}
