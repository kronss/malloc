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

    	ptr0 = malloc(3);
		ptr1 = malloc(3);
		ptr2 = malloc(3);
		ptr3 = malloc(3);

    show_alloc_mem();

    printf("---------------------------------------------------------\n");
////
////	  printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr0);
////	  printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr1);
    	free(ptr0);
    	free(ptr1);
////    	free(ptr2);
        show_alloc_mem();
//
printf("---------------------------------------------------------\n");
//    	ptr0 = malloc(1);
//
	ptr1 = realloc(ptr0, 150);
//    show_alloc_mem();

//    free(NULL + 0x1);
//    ptr = malloc(3111);
//
    show_alloc_mem();

//    printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr);
//    ptr = malloc(4);
//    printf("%s:%d: ----- %llu\n", __func__, __LINE__, ptr);



    return (0);
}
