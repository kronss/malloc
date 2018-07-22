#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

pthread_mutex_t mutex_malloc = PTHREAD_MUTEX_INITIALIZER;
struct malloc_meneger_s malloc_meneger_g =
{
    .zone_heads = {NULL, NULL, NULL}
};

void *get_ptr_from_zone(size_t size, zone_type_e zone)
{
    void *retval;

    if (!malloc_meneger_g.zone_heads[zone]) {
        malloc_meneger_g.zone_heads[zone] = new_zone();
        if (!malloc_meneger_g.zone_heads[zone]) {
            return NULL;
        }
    }
    retval = get_best_chunk();

    return retval;
}




void *get_ptr(size)
{
    void *retval;

    if (size < TINY_TRESHHOLD) {
        retval = get_ptr_from_zone(size, TINY);
    } else if (size < SMALL_TRESHHOLD) {
        retval = get_ptr_from_zone(size, SMALL);
    } else if (size < LARGE_TRESHHOLD) {
        retval = get_ptr_from_zone(size, LARGE);
    } else {
        ;
    }


    return retval;
}



void *malloc(size_t size)
{
	void *retval;

	pthread_mutex_lock(&mutex_malloc);
	retval = NULL;
	if (size <= 0) {
	    goto end;
	}

	if (size < (size_t)-32) {
		ALIGN_META_INFO(size);
	}

    get_ptr(size);


	if (NULL == retval) {
//		errno = ENOMEM;

	}

end:
    pthread_mutex_unlock(&mutex_malloc);
    return retval;
}
