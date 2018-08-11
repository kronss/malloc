#include "malloc_internal_api.h"

//int need_realloc_in_same_zone(struct zone_s *zone_ptr, struct block_s *block_ptr, size_t new_size)
int need_realloc_in_same_zone(struct block_s *block_ptr, size_t new_size)
{
	int ret_val = 0;

	if (get_zone_head(block_ptr->alloc_size) == get_zone_head(new_size)) {
		ret_val = 1;
	}
	return ret_val;
}


int can_be_expanded(struct block_s *block_ptr, size_t new_size)
{
	int ret_val = 0;
	struct block_s *next_block_ptr = block_ptr->next;

	if (!next_block_ptr || next_block_ptr->free)
		goto end;

	if (new_size < (block_ptr->alloc_size + next_block_ptr->alloc_size - METABLOCK_SIZE)) {
		ret_val = 1;
	}

end:
	return ret_val;
}

void *do_realloc_in_same_zone(struct zone_s *zone_ptr, struct block_s *old_block_ptr, size_t new_size)
{
	void *new_block_ptr = NULL;

	if (can_be_expanded(old_block_ptr, new_size)) {
		/*Expand zone*/
		zone_ptr->space_left += old_block_ptr->alloc_size;
		old_block_ptr->alloc_size = new_size;
		zone_ptr->space_left -= old_block_ptr->alloc_size;
		new_block_ptr = old_block_ptr; /*same_ptr*/
	} else {
		new_block_ptr = get_ptr(new_size);
		ft_memmove(new_block_ptr, old_block_ptr, old_block_ptr->alloc_size);
		free_defragment_unmap(zone_ptr, old_block_ptr);
	}

	return new_block_ptr;
}







static void *do_realloc(struct zone_s *zone_ptr, struct block_s *block_ptr, size_t new_size)
{
	void *ret_val = NULL;

//	if (need_realloc_in_same_zone(zone_ptr, block_ptr, new_size)) {
	if (need_realloc_in_same_zone(block_ptr, new_size)) {
		do_realloc_in_same_zone(zone_ptr, block_ptr, new_size);
	}
	else {
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

	ret_val = get_ptr(size);
bad:
    pthread_mutex_unlock(&mutex_malloc);
end:
    return ret_val;
}
