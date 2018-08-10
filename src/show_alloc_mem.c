#include "malloc_internal_api.h"
#include <stdio.h> //printf


static int total_alloc;


static inline const char *get_zone_type(zone_type)
{
	const char *retval;

	switch (zone_type) {
		case TINY:  retval = "TINY" ; break;
		case SMALL: retval = "SMALL"; break;
		case LARGE: retval = "LARGE"; break;
		default:    retval = "N/A"  ; break;
	}
	return retval;
}

void show_meta_block(struct block_s * block_ptr)
{
	long diff;
	while(block_ptr)
	{
		if (!block_ptr->free) {

			diff = (int8_t *)block_ptr->next - (int8_t *)block_ptr;
			printf("\t %p - %p : %ld bytes\n", block_ptr, block_ptr->next, diff);

			total_alloc += diff;
		}
		else {
			printf("\t %p - %p : %ld bytes are free\n", block_ptr, block_ptr->next, block_ptr->alloc_size);
		}
		block_ptr = block_ptr->next;
	}
}



void show_alloc_zone(enum zone_type_e zone_type)
{
	struct zone_s *zone_ptr;

	zone_ptr = malloc_meneger_g.zone_heads[zone_type];
	while (zone_ptr) {
		printf("%s : %p\n", get_zone_type(zone_type), zone_ptr);

		show_meta_block(zone_ptr->md_block_head);
		zone_ptr = zone_ptr->next;
	}
}

void show_alloc_mem()
{
	enum zone_type_e zone_type;

	pthread_mutex_lock(&mutex_malloc);
	total_alloc = 0;
	zone_type = MIN_ZONE_TYPE;
	while (zone_type < MAX_ZONE_TYPE) {
		show_alloc_zone(zone_type);
		++zone_type;
	}
	printf("Total : %d bytes\n", total_alloc);

	pthread_mutex_unlock(&mutex_malloc);
}
