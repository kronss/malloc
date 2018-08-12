#include "malloc_internal_api.h"

int is_all_blocks_free(struct zone_s *zone_ptr)
{
	int ret_val = 1;
	struct block_s *block_ptr = &zone_ptr->md_block_head;

	while (block_ptr) {
		if (block_ptr->free == 0) {
			ret_val = 0;
			goto end;
		}
		block_ptr = block_ptr->next;
	}
end:
	return ret_val;
}

void remove_cur_zone_to_pull(struct zone_s *zone_ptr)
{
    enum zone_type_e zone_type = MIN_ZONE_TYPE;
	struct zone_s *zone_next = zone_ptr->next;
	struct zone_s *zone_prev = zone_ptr->prev;

	if (zone_prev)
		zone_prev->next = zone_next;
	else
	{
		while (zone_type < MAX_ZONE_TYPE)
		{
			if (malloc_meneger_g.zone_heads[zone_type] == zone_ptr)
				malloc_meneger_g.zone_heads[zone_type] = zone_next;
			++zone_type;
		}
	}
	if (zone_next)
		zone_prev->prev = zone_prev;
	munmap(zone_ptr, zone_ptr->origin_size); // + sizeof(t_page));
}

void free_defragment_unmap(struct zone_s *zone_ptr, struct block_s *block_ptr)
{
	free_and_return_block_to_pull(zone_ptr, block_ptr);
	if (is_all_blocks_free(zone_ptr)) {
		remove_cur_zone_to_pull(zone_ptr);
	}
}
