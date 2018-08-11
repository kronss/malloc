
#include "malloc_internal_api.h"

















void *realloc()
{
	void *ret_val;

	pthread_mutex_lock(&mutex_malloc);
	ret_val = NULL;
	if (size <= 0) {
	    goto end;
	}
	printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	if (size < (size_t) (-sizeof(struct block_s))) {
		ALIGN_META_INFO(size);
//		printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	}
	printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	ret_val = get_ptr(size);
end:
    pthread_mutex_unlock(&mutex_malloc);
    return ret_val;
}
