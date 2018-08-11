#include "malloc_internal_api.h"

void free(void *ptr)
{
	struct zone_s *zone_ptr = NULL;
	struct block_s *block_ptr = NULL;

	pthread_mutex_lock(&mutex_malloc);
	if (ptr == NULL) {
		goto end;
	}
//	printf("%s:%d: ptr %p\n", __func__, __LINE__, ptr); //debug
	block_ptr = get_ptr_to_md(ptr);
//	printf("%s:%d: ptr %p\n", __func__, __LINE__, ptr); //debug

	if (validate_md(&zone_ptr, &block_ptr)) {
		goto end;
	}
	free_defragment_unmap(zone_ptr, block_ptr);

//	printf("%s:%d: bingo! %p\n", __func__, __LINE__, ptr); //debug
end:
	pthread_mutex_unlock(&mutex_malloc);
}
