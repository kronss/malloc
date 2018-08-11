//#include <unistd.h>
#include "malloc_internal_api.h"



//static inline void *init_new_meta_block(struct zone_s *block_ptr, size_t size);
void *find_available_block(struct zone_s *zone_ptr, size_t size);
void *find_available_zone(size_t size_request, struct zone_s *zone_head, enum zone_type_e zone_type);
void *init_new_zone(enum zone_type_e zone_type, struct zone_s *prev_zone);
void *get_ptr_from_zone(size_t size, enum zone_type_e zone_type);
void *get_ptr(size_t size);
void *malloc(size_t size);




void *create_new_block(struct zone_s *zone_ptr,
                       struct block_s *block_ptr,
                       size_t size)
{
    struct block_s *new_block_ptr;

    new_block_ptr = (struct block_s *)((uint8_t*)block_ptr + size);
    new_block_ptr->alloc_size = block_ptr->alloc_size - size;
    printf("%s:%d:%zu\n", __func__, __LINE__, new_block_ptr->alloc_size);
    new_block_ptr->free = 1;
    new_block_ptr->next = block_ptr->next;
    new_block_ptr->prev = block_ptr;

    block_ptr->alloc_size = size;
    block_ptr->free = 0;
    block_ptr->next = new_block_ptr;
//    block_ptr->prev =

    zone_ptr->space_left -= size;

    printf("%s:%d:%p\n", __func__, __LINE__, block_ptr->data);
    return block_ptr->data;
}

static inline
int check_curr_block(struct zone_s *zone_ptr,
                     struct block_s *block_ptr, size_t size)
{
    struct block_s *next;
    unsigned long space_between;
    int retval = 0;

    if (!block_ptr->free) {
//        retval = false
        goto end;
    }
    if (!block_ptr->next) {
        next = zone_ptr->md_block_head + zone_ptr->origin_size;
//    	next = zone_ptr->md_block_head + zone_ptr->space_left;
    } else {
        next = block_ptr->next;
    }
    space_between = (unsigned long) (next - block_ptr);
    if (size < space_between) {
        retval = 1;
    }
end:
    return retval;
}

void *find_available_block(struct zone_s *zone_ptr, size_t size)
{
	void *retval = NULL;
    struct block_s *block_ptr = zone_ptr->md_block_head;
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
	return retval;
}




void *find_available_zone(size_t size_request, struct zone_s *zone_head, enum zone_type_e zone_type)
{
	struct zone_s *zone_ptr;
	void *retval = NULL;

	zone_ptr = zone_head;
	while (zone_ptr) {
		if (size_request < zone_ptr->space_left) {
//			retval = init_new_meta_block(zone_ptr, size_request);
			retval = find_available_block(zone_ptr, size_request);
			break ;
		}
		if (zone_ptr->next) {
			zone_ptr = zone_ptr->next;
		} else {
			zone_ptr->next = init_new_zone(zone_type, zone_ptr);
			if (!zone_ptr->next) {
				retval = NULL;
				break ;
			}
			zone_ptr = zone_ptr->next;
		}
	}
	return retval;
}



void *init_new_zone(enum zone_type_e zone_type, struct zone_s *prev_zone)
{
    void           *raw_ptr;
    struct zone_s  *zone_ptr;
    struct block_s *block_ptr;
    size_t          size;

    switch (zone_type) {
    case TINY:  size = TINY_ZONE  ;    break;
    case SMALL: size = SMALL_ZONE ;    break;
    case LARGE: size = LARGE_ZONE ;    break;

    default:    raw_ptr = NULL    ;    goto end;
    }
    ALIGN_PAGE_SIZE(size);
    printf("%s:%d: page size %zu\n", __func__, __LINE__, size);

    raw_ptr = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (raw_ptr == MAP_FAILED) {
		raw_ptr = NULL;
		goto end;
	}

	zone_ptr = (struct zone_s *)raw_ptr;
	zone_ptr->origin_size = size;
	zone_ptr->space_left = size - sizeof(struct zone_s) - sizeof(struct block_s);
	zone_ptr->next = NULL;
	zone_ptr->prev = prev_zone;
	if (prev_zone) {
		prev_zone->next = (struct zone_s *)raw_ptr;
	}
//	init_head_md();
	block_ptr = zone_ptr->md_block_head;
    block_ptr->next = NULL;
    block_ptr->prev = NULL;
    block_ptr->alloc_size = zone_ptr->space_left;
    block_ptr->free = 1;
    printf("%s:%d: ----- %llu\n", __func__, __LINE__, raw_ptr);
end:
    return raw_ptr ;
}

void *get_ptr_from_zone(size_t size, enum zone_type_e zone_type)
{
    void *retval;

    if (!malloc_meneger_g.zone_heads[zone_type]) {
        malloc_meneger_g.zone_heads[zone_type] = init_new_zone(zone_type, NULL);
        if (!malloc_meneger_g.zone_heads[zone_type]) {
            return NULL;
        }
    }
	printf("%s:%d\n", __func__, __LINE__); //debug

    retval = find_available_zone(size, malloc_meneger_g.zone_heads[zone_type], zone_type);

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
	printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	if (size < (size_t) (-sizeof(struct block_s))) {
		ALIGN_META_INFO(size);
//		printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	}
	printf("%s:%d:size == %zu\n", __func__, __LINE__, size); //debug
	retval = get_ptr(size);
end:
    pthread_mutex_unlock(&mutex_malloc);
    return retval;
}
