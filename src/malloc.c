#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

pthread_mutex_t mutex_malloc = PTHREAD_MUTEX_INITIALIZER;
struct malloc_meneger_s malloc_meneger_g =
{
    .zone_heads = {NULL, NULL, NULL}
};

//TODO: creat if for first initialization
static inline void init_new_meta_block(struct block_s *block_ptr, size_t size)
{
	ft_memset(block_ptr, 0x0, sizeof(struct block_s));
	//TODO:init meta data block
}

void *get_best_chunk(size, struct block_s *head, zone_type_e zone)
{
	struct block_s *i_ptr;
	void *retval;

	i_ptr = head;
	while (i_ptr) {
		if (i_ptr->space_left > size) {
			retval = init_new_meta_block(i_ptr, size);
			break ;
		}
		if (i_ptr->next) {
			i_ptr = i_ptr->next;
		} else {
			i_ptr->next = init_new_page(zone);
			if (!i_ptr->next) {
				retval = NULL;
				break ;
			}
			i_ptr = i_ptr->next;
		}
	}
	return retval;
}



void *init_new_page(zone_type_e zone)
{
    void           *raw_ptr;
    struct block_s *block;
    size_t          size;

    switch (zone) {
    case TINY:  size = TINY_ZONE  ;    break;
    case SMALL: size = SMALL_ZONE ;    break;
    case LARGE: size = LARGE_ZONE ;    break;

    default:    raw_ptr = NULL    ; goto bad;
    }
    ALIGN_PAGE_SIZE(size);
    raw_ptr = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (raw_ptr == MAP_FAILED) {
		raw_ptr = NULL;
		goto bad;
	}
	init_new_meta_block((struct block_s *)raw_ptr, size);

bad:
    return raw_ptr ;
}

void *get_ptr_from_zone(size_t size, zone_type_e zone)
{
    void *retval;

    if (!malloc_meneger_g.zone_heads[zone]) {
        malloc_meneger_g.zone_heads[zone] = init_new_page(zone);
        if (!malloc_meneger_g.zone_heads[zone]) {
            return NULL;
        }
    }

    retval = get_best_chunk(size, malloc_meneger_g.zone_heads[zone], zone);


    return retval;
}




void *get_ptr(size)
{
    void *retval;

    if (size < TINY_TRESHHOLD) {
        retval = get_ptr_from_zone(size, TINY);
    } else if (size < SMALL_TRESHHOLD) {
        retval = get_ptr_from_zone(size, SMALL);
//    } else if (size < LARGE_TRESHHOLD) {
//        retval = get_ptr_from_zone(size, LARGE);
    } else {
        retval = get_ptr_from_zone(size, LARGE);
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
