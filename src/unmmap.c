#include "malloc_internal_api.h"

static int is_all_blocks_free(struct s_zone *zone_ptr);
static void remove_cur_zone_to_pull(struct s_zone *zone_ptr);



static int is_all_blocks_free(struct s_zone *zone_ptr)
{
	int ret_val = 1;
	struct s_block *block_ptr = &zone_ptr->md_block_head;

	while (block_ptr) {
		if (block_ptr->free == 0) {
			ret_val = 0;
			goto end;
		}
		block_ptr = block_ptr->next;
	    printf("%s:%d: ----- %p\n", __func__, __LINE__, block_ptr);

	}
end:
	return ret_val;
}

static void remove_cur_zone_to_pull(struct s_zone *zone_ptr)
{
    enum e_zone_type zone_type = MIN_ZONE_TYPE;
	struct s_zone *zone_next = zone_ptr->next;
	struct s_zone *zone_prev = zone_ptr->prev;

    printf("%s:%d: ****************************\n", __func__, __LINE__);
    printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr);
	printf("%s:%d: ----- %zu\n", __func__, __LINE__, zone_ptr->origin_size);
	printf("%s:%d: ----- %zu\n", __func__, __LINE__, zone_ptr->space_left);
	printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr->next);
	printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr->prev);
	printf("%s:%d: --111 %p\n", __func__, __LINE__, zone_ptr->md_block_head);
//	printf("%s:%d: --112 %p\n", __func__, __LINE__, &(zone_ptr->md_block_head));

	if (zone_prev)
		zone_prev->next = zone_next;
	else
	{
		while (zone_type < MAX_ZONE_TYPE)
		{
			if (g_alloc_mnr.zone_heads[zone_type] == zone_ptr) {
			    printf("%s:%d: %p\n", __func__, __LINE__, g_alloc_mnr.zone_heads[zone_type]);
			    printf("%s:%d: %p\n", __func__, __LINE__, zone_ptr);

			    g_alloc_mnr.zone_heads[zone_type] = zone_next;
			}
			++zone_type;
		}
	}
    printf("%s:%d: ****************************\n", __func__, __LINE__);
    printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr);
	printf("%s:%d: ----- %zu\n", __func__, __LINE__, zone_ptr->origin_size);
	printf("%s:%d: ----- %zu\n", __func__, __LINE__, zone_ptr->space_left);
	printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr->next);
	printf("%s:%d: ----- %p\n", __func__, __LINE__, zone_ptr->prev);
	printf("%s:%d: --111 %p\n", __func__, __LINE__, zone_ptr->md_block_head);
//	printf("%s:%d: --112 %p\n", __func__, __LINE__, &(zone_ptr->md_block_head));
	if (zone_prev) /* main fix probably there TODO: check*/
		zone_prev->prev = zone_prev;
    printf("%s:%d: ----- %zu\n", __func__, __LINE__, zone_ptr->origin_size);
//    sleep(1);
	munmap(zone_ptr, zone_ptr->origin_size);
}

void free_defragment_unmap(struct s_zone *zone_ptr, struct s_block *block_ptr)
{
	free_and_return_block_to_pull(zone_ptr, block_ptr);
	if (is_all_blocks_free(zone_ptr)) {
	    //TODO:: if last zone Do not unmmap it. It cause context switches, cause for  increasing latency
		remove_cur_zone_to_pull(zone_ptr);
	}
}
