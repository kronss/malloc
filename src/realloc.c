#include "malloc_internal_api.h"

void *do_realloc_in_same_zone(struct zone_s *zone_ptr, struct block_s *old_block_ptr, size_t new_size);


//int need_realloc_in_same_zone(struct zone_s *zone_ptr, struct block_s *block_ptr, size_t new_size)
int need_realloc_in_same_zone(struct block_s *block_ptr, size_t new_size)
{
	int ret_val = 0;

	printf("old == %p\n", get_zone_head(block_ptr->alloc_size)); //debug
	printf("new == %p\n", get_zone_head(new_size)); //debug

	if (get_zone_head(block_ptr->alloc_size) == get_zone_head(new_size)) {
		ret_val = 1;
	}
	return ret_val;
}


int can_be_expanded(struct block_s *block_ptr, size_t new_size)
{
	int ret_val = 0;
	struct block_s *next_block_ptr = block_ptr->next;

	if (!next_block_ptr || !next_block_ptr->free)
		goto end;
//	printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug
//	printf("%s:%d:new_size == %zu\n", __func__, __LINE__, (block_ptr->alloc_size + next_block_ptr->alloc_size - METABLOCK_SIZE)); //debug
	if (new_size <= (block_ptr->alloc_size + next_block_ptr->alloc_size - METABLOCK_SIZE)) {
		ret_val = 1;
	}

end:
//	printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug
printf("%s:%d:ret_val == %d\n", __func__, __LINE__, ret_val); //debug
	return ret_val;
}


void *expand_curr_block(struct zone_s *zone_ptr,
                       struct block_s *block_ptr,
                       size_t new_size)
{
    struct block_s *old_next_block_ptr = block_ptr->next;
    struct block_s *next_block_ptr = NULL;

	zone_ptr->space_left += block_ptr->alloc_size;

	next_block_ptr = (struct block_s *)((uint8_t *)block_ptr + new_size);

//    printf("%s:%d:%p\n", __func__, __LINE__, block_ptr);
//    printf("%s:%d:%p\n", __func__, __LINE__, next_block_ptr);
//    printf("%s:%d:%zu\n", __func__, __LINE__, new_block_ptr->alloc_size);
    next_block_ptr->alloc_size = old_next_block_ptr->alloc_size - (new_size - block_ptr->alloc_size);
//    printf("%s:%d:%zu\n", __func__, __LINE__, new_block_ptr->alloc_size);
    next_block_ptr->free = 1;
    next_block_ptr->next = old_next_block_ptr->next;
    next_block_ptr->prev = block_ptr;

    block_ptr->alloc_size = new_size;
    block_ptr->free = 0;
    block_ptr->next = next_block_ptr;
//    printf("%s:%d:%p\n", __func__, __LINE__, block_ptr->next);
//    printf("%s:%d:%p\n", __func__, __LINE__, next_block_ptr);
//    block_ptr->prev =

    zone_ptr->space_left -= block_ptr->alloc_size;

    printf("%s:%d:%p\n", __func__, __LINE__, block_ptr);
    return block_ptr;
}





void *do_realloc_in_same_zone(struct zone_s *zone_ptr, struct block_s *old_block_ptr, size_t new_size)
{
//	struct block_s *new_block_ptr = NULL;
	void *new_block_ptr = NULL;


	printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug

	if (can_be_expanded(old_block_ptr, new_size)) {
		/*Expand zone*/
		  printf("%s:%d: old_block_ptr       == %p\n", __func__, __LINE__, old_block_ptr);
		  printf("%s:%d: old_block_ptr->next == %p\n", __func__, __LINE__, old_block_ptr->next);
		  printf("%s:%d: new_block_ptr       == %p\n", __func__, __LINE__, new_block_ptr);
//		  printf("%s:%d: new_block_ptr->next == %p\n", __func__, __LINE__, new_block_ptr->next);


//		new_block_ptr->alloc_size = old_block_ptr->alloc_size + old_block_ptr->next->alloc_size - METABLOCK_SIZE;
		new_block_ptr = expand_curr_block(zone_ptr, old_block_ptr, new_size);

//		((struct block_s *)new_block_ptr)->next = old_block_ptr->next->next;
//		new_block_ptr->alloc_size = new_size;
//		zone_ptr->space_left -= new_block_ptr->alloc_size;
		  printf("%s:%d: old_block_ptr       == %p\n", __func__, __LINE__, old_block_ptr);
		  printf("%s:%d: old_block_ptr->next == %p\n", __func__, __LINE__, old_block_ptr->next);
		  printf("%s:%d: new_block_ptr       == %p\n", __func__, __LINE__, new_block_ptr);
		  printf("%s:%d: new_block_ptr->next == %p\n", __func__, __LINE__, ((struct block_s *)new_block_ptr)->next);
//		((struct block_s *)new_block_ptr)->next = old_block_ptr->next;
	} else {
		printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug

		new_block_ptr = get_ptr(new_size);
		printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug
		ft_memmove(new_block_ptr, old_block_ptr, old_block_ptr->alloc_size);
		printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug
		free_defragment_unmap(zone_ptr, old_block_ptr);
		printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug
	}
	printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug
	return (void *)new_block_ptr;
}







static void *do_realloc(struct zone_s *zone_ptr, struct block_s *block_ptr, size_t new_size)
{
	void *ret_val = NULL;

//	if (need_realloc_in_same_zone(zone_ptr, block_ptr, new_size)) {
	if (need_realloc_in_same_zone(block_ptr, new_size)) {
		printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug
		ret_val = do_realloc_in_same_zone(zone_ptr, block_ptr, new_size);
	}
	else {
		printf("%s:%d:new_size == %zu\n", __func__, __LINE__, new_size); //debug
		ret_val = get_ptr(new_size);
		ft_memmove(ret_val, block_ptr, block_ptr->alloc_size);
		free_defragment_unmap(zone_ptr, block_ptr);
	}

	return ret_val;
}





void *realloc(void *ptr, size_t size)
{
	void *ret_val = NULL;
    struct block_s *block_ptr = NULL;
    struct zone_s *zone_ptr = NULL;

	if (ptr == NULL) {
		ret_val = (malloc(size));
		goto end;
	}
	pthread_mutex_lock(&mutex_malloc);
	if (size <= 0) {
	    goto bad;
	}

	block_ptr = get_ptr_to_md(ptr);
	if (validate_md(&zone_ptr, &block_ptr)) {
		goto bad;
	}
//	printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	if (size < (size_t) (-sizeof(struct block_s))) {
		ALIGN_META_INFO(size);
//		printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	}
	printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug

	ret_val = do_realloc(zone_ptr, block_ptr, size);

//	ret_val = get_ptr(size);
	ret_val += ((size_t)&((struct block_s *)0)->data);

bad:
    pthread_mutex_unlock(&mutex_malloc);
end:
    return ret_val;
}
