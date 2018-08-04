#include <malloc.h>
#include <stddef.h> /*offsetof*/






int validate_md(struct block_s *block_ptr)
{
	int ret_val;
	struct zone_s *zone_ptr;

	get_zone_ptr(block_ptr->alloc_size);




	return (0);
}






void *get_ptr_to_md(void *ptr)
{
	void *ret_val;

	ret_val = ptr - offsetof(struct block_s, data);
	return ret_val;
}

void free(void *ptr)
{
	pthread_mutex_lock(&mutex_malloc);
	if (ptr < (NULL + sizeof(struct zone_s) + sizeof(struct block_s))) {
		goto end;
	}

	printf("%s:%d: ptr %p\n", __func__, __LINE__, ptr); //debug
	ptr = get_ptr_to_md(ptr);
	printf("%s:%d: ptr %p\n", __func__, __LINE__, ptr); //debug

	if (!validate_md(ptr)) {
		goto end;
	}





end:
	pthread_mutex_unlock(&mutex_malloc);
}
