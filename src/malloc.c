//#include <unistd.h>
#include "malloc_internal_api.h"



//static inline void *init_new_meta_block(struct zone_s *block_ptr, size_t size);
void *find_available_block(struct zone_s *zone_ptr, size_t size);
void *find_available_zone(size_t size_request, struct zone_s *zone_head, enum zone_type_e zone_type);
void *init_new_zone(enum zone_type_e zone_type, struct zone_s *prev_zone, size_t size_request);
void *get_ptr_from_zone(size_t size, enum zone_type_e zone_type);
void *get_ptr(size_t size);
void *malloc(size_t size);




void *create_new_block(struct zone_s *zone_ptr,
                       struct block_s *block_ptr,
                       size_t size)
{
    struct block_s *next_block_ptr;

    next_block_ptr = (struct block_s *)((uint8_t *)block_ptr + size);
//    printf("%s:%d:%p\n", __func__, __LINE__, block_ptr);
//    printf("%s:%d:%p\n", __func__, __LINE__, next_block_ptr);
//    printf("%s:%d:%zu\n", __func__, __LINE__, new_block_ptr->alloc_size);
    next_block_ptr->alloc_size = block_ptr->alloc_size - size;
//    printf("%s:%d:%zu\n", __func__, __LINE__, new_block_ptr->alloc_size);
    next_block_ptr->free = 1;
    next_block_ptr->next = block_ptr->next;
    next_block_ptr->prev = block_ptr;

    block_ptr->alloc_size = size;
    block_ptr->free = 0;
    block_ptr->next = next_block_ptr;
//    printf("%s:%d:%p\n", __func__, __LINE__, block_ptr->next);
//    printf("%s:%d:%p\n", __func__, __LINE__, next_block_ptr);
//    block_ptr->prev =

    zone_ptr->space_left -= size;

//    printf("%s:%d:%p\n", __func__, __LINE__, block_ptr);
    return block_ptr;
}

/*
 * checck current block. Can we insert it
 */

static inline
int check_curr_block(struct zone_s *zone_ptr,
                     struct block_s *block_ptr, size_t size_request)
{
	int ret_val = 0;

	if (size_request < block_ptr->alloc_size) {
		ret_val = 1;
	}

	return ret_val;
}


//static inline
//int check_curr_block(struct zone_s *zone_ptr,
//                     struct block_s *block_ptr, size_t size_request)
//{
//    struct block_s *next;
//    unsigned long space_between = 0;
//    int retval = 0;
////    printf("%s:%d: *** size  *****************************************\n");
//
////    printf("%s:%d: free == %d\n", block_ptr->free - zone_ptr);
//    if (!block_ptr->free) {
////        retval = false
//        goto end;
//    }
//    if (!block_ptr->next) {
//    	printf("---------------------onedick-----------------\n");
//        next = (uint8_t *)zone_ptr + zone_ptr->origin_size;// - sizeof(struct zone_s);
////        printf("%s:%d: *** zone_ptr              == %p\n",  __func__, __LINE__, zone_ptr);
////        printf("%s:%d: *** next                  == %p\n",  __func__, __LINE__, next);
////        next = (uint8_t)&zone_ptr->md_block_head + (uint8_t)zone_ptr->origin_size;// - sizeof(struct zone_s);
////    	next = &zone_ptr->md_block_head + zone_ptr->space_left;
//    } else {
//    	printf("---------------------twodick-----------------\n");
//        next = block_ptr->next;
//    }
//    space_between = (unsigned long) ((uint8_t)next - (uint8_t)block_ptr);
//    if (size_request < space_between) {
//        retval = 1;
//    }
////	printf("---------------------noend-----------------\n");
//end:
//	printf("%s:%d: *** sizeof(zone_ptr)      == %zu\n",  __func__, __LINE__, sizeof(*zone_ptr));
////	printf("%s:%d: *** zone_ptr              == %p\n",  __func__, __LINE__, zone_ptr);
//	printf("%s:%d: *** zone_ptr->origin_size == %zu\n",  __func__, __LINE__, zone_ptr->origin_size);
//	printf("%s:%d: *** zone_ptr->space_left  == %zu\n",  __func__, __LINE__, zone_ptr->space_left);
////	printf("%s:%d: *** zone_ptr->next        == %p\n",  __func__, __LINE__, &zone_ptr->next);
////	printf("%s:%d: *** zone_ptr->prev        == %p\n",  __func__, __LINE__, &zone_ptr->prev);
////	printf("%s:%d: *** zone_ptr->md_block_he == %p\n",  __func__, __LINE__, &zone_ptr->md_block_head);
////	printf("%s:%d: *** block_ptr->prev       == %p\n",  __func__, __LINE__, block_ptr->prev);
////	printf("%s:%d: *** block_ptr->next       == %p\n",  __func__, __LINE__, block_ptr->next);
//	printf("%s:%d: *** block_ptr->free       == %d\n",  __func__, __LINE__, block_ptr->free);
//    printf("%s:%d: *** size                  == %zu\n", __func__, __LINE__, size_request);
//    printf("%s:%d: *** space_between         == %zu\n", __func__, __LINE__, (size_t)space_between);
//    printf("%s:%d: *** zone_ptr->origin_size == %zu\n", __func__, __LINE__, zone_ptr->origin_size);
//    printf("%s:%d: ***   retval              == %d\n" , __func__, __LINE__, retval);
//    return retval;
//}

void *find_available_block(struct zone_s *zone_ptr, size_t size)
{
	void *retval = NULL;
    struct block_s *block_ptr = &zone_ptr->md_block_head;
//
//    if (!block_ptr) {
//        init_head_block(zone_ptr, block_ptr, size);
//    }
    while (block_ptr) {
//        block_ptr = block_ptr->next ?: block_ptr;

        if (check_curr_block(zone_ptr, block_ptr, size)) {
            retval = create_new_block(zone_ptr, block_ptr, size);
            goto end;
        }

        block_ptr = block_ptr->next;
    }
end:
	printf("%s:%d: new_block_ptr       == %p\n", __func__, __LINE__, retval);
	return retval;
}




void *find_available_zone(size_t size_request, struct zone_s *zone_head, enum zone_type_e zone_type)
{
	struct zone_s *zone_ptr;
	void *retval = NULL;

	zone_ptr = zone_head;
	while (zone_ptr) {
	    printf("%s:%d: size_request          %zu\n", __func__, __LINE__, size_request);
	    printf("%s:%d: zone_ptr->space_left) %zu\n", __func__, __LINE__, zone_ptr->space_left);
		if (size_request < zone_ptr->space_left) {
//			retval = init_new_meta_block(zone_ptr, size_request);
			retval = find_available_block(zone_ptr, size_request);
			break ;
		}
		if (zone_ptr->next) {
			zone_ptr = zone_ptr->next;
		} else {
			zone_ptr->next = init_new_zone(zone_type, zone_ptr, size_request);
			if (!zone_ptr->next) {
				retval = NULL;
				break ;
			}
			zone_ptr = zone_ptr->next;
		}
	}
	return retval;
}



void *init_new_zone(enum zone_type_e zone_type, struct zone_s *prev_zone, size_t size_request)
{
    void           *raw_ptr;
    struct zone_s  *zone_ptr;
    struct block_s *block_ptr;
    size_t          size_zone;
    static int ii = 1;


    switch (zone_type) {
    case TINY:  size_zone = TINY_ZONE  ;    break;
    case SMALL: size_zone = SMALL_ZONE ;    break;
    case LARGE: size_zone = ALIGN_PAGE_SIZE(size_request) ;    break;

    default:    raw_ptr = NULL    ;    goto end;
    }
    ALIGN_PAGE_SIZE(size_zone);
    printf("%s:%d: size_zone %zu\n", __func__, __LINE__, size_zone);

    raw_ptr = mmap(NULL, size_zone, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (raw_ptr == MAP_FAILED) {
		raw_ptr = NULL;
		goto end;
	}

	zone_ptr = (struct zone_s *)raw_ptr;
	zone_ptr->origin_size = size_zone;
	zone_ptr->space_left = size_zone - sizeof(struct zone_s);
	zone_ptr->next = NULL;
	zone_ptr->prev = prev_zone;
	if (prev_zone) {
		prev_zone->next = zone_ptr;
	}
//	init_head_md();
	block_ptr = &(zone_ptr->md_block_head);
    block_ptr->next = NULL;
    block_ptr->prev = NULL;
    block_ptr->alloc_size = zone_ptr->space_left;
    block_ptr->free = 1;
end:
	if (ii) {
    printf("%s:%d: ****************************\n", __func__, __LINE__);
    printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr);
	printf("%s:%d: ----- %zu\n", __func__, __LINE__, zone_ptr->origin_size);
	printf("%s:%d: ----- %zu\n", __func__, __LINE__, zone_ptr->space_left);
	printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr->next);
	printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr->prev);
	printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr->md_block_head);
	printf("%s:%d: ----- %p\n", __func__, __LINE__, &(zone_ptr->md_block_head));

	ii--;
	}
    return raw_ptr ;
}

void *get_ptr_from_zone(size_t size, enum zone_type_e zone_type)
{
    void *retval;

    printf("%s:%d\n", __func__, __LINE__); //debug
    if (!malloc_meneger_g.zone_heads[zone_type]) {
    	printf("%s:%d\n", __func__, __LINE__); //debug
        malloc_meneger_g.zone_heads[zone_type] = init_new_zone(zone_type, NULL, size);
    	printf("%s:%d\n", __func__, __LINE__); //debug
        if (!malloc_meneger_g.zone_heads[zone_type]) {
        	retval = NULL;
        	goto end;
        }
    }
	printf("%s:%d\n", __func__, __LINE__); //debug

    retval = find_available_zone(size, malloc_meneger_g.zone_heads[zone_type], zone_type);

end:
    return retval;
}

void *get_ptr(size_t size)
{
    void *retval;

    if (size <= TINY_TRESHHOLD) {
        retval = get_ptr_from_zone(size, TINY);
    } else if (size <= SMALL_TRESHHOLD) {
        retval = get_ptr_from_zone(size, SMALL);
    } else {
        retval = get_ptr_from_zone(size, LARGE); /*rework check according to subj*/
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
//	printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	if (size < (size_t) (-sizeof(struct block_s))) {
		ALIGN_META_INFO(size);
//		printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	}
//	printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	retval = get_ptr(size);
	retval += ((size_t)&((struct block_s *)0)->data);
end:
    pthread_mutex_unlock(&mutex_malloc);
    return retval;
}
