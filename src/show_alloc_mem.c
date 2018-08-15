#include "malloc_internal_api.h"

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

void show_meta_block(struct s_block * block_ptr)
{
	long diff;
	while(block_ptr)
	{
		diff = (int8_t *)block_ptr->next - (int8_t *)block_ptr; //tmp moved here
		if (!block_ptr->free) {
//			diff = (int8_t *)block_ptr->next - (int8_t *)block_ptr;
			printf("\t %p - %11p : diff %10ld : %zu bytes\n", block_ptr, block_ptr->next, diff, block_ptr->alloc_size);
			g_alloc_mnr.print_total_alloc += diff;
		}
		else {
			printf("\t %p - %11p : diff %10ld : %zu bytes are freed\n", block_ptr, block_ptr->next, diff, block_ptr->alloc_size);
		}
		block_ptr = block_ptr->next;
//		sleep(1);
	}
}

void show_alloc_zone(enum e_zone_type zone_type)
{
	struct s_zone *zone_ptr;

	zone_ptr = g_alloc_mnr.zone_heads[zone_type];
	while (zone_ptr) {
		printf("%s : %p\n", get_zone_type(zone_type), zone_ptr);

		show_meta_block(&zone_ptr->md_block_head);
		zone_ptr = zone_ptr->next;
	}
}

void show_alloc_mem()
{
	enum e_zone_type zone_type;

	pthread_mutex_lock(&mutex_malloc);
	g_alloc_mnr.print_total_alloc = 0;
	zone_type = MIN_ZONE_TYPE;
	while (zone_type < MAX_ZONE_TYPE) {
		show_alloc_zone(zone_type);
		++zone_type;
	}
	printf("Total : %zu bytes\n", g_alloc_mnr.print_total_alloc);

	pthread_mutex_unlock(&mutex_malloc);
}
