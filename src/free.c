#include "malloc_internal_api.h"

int check_block_ptr(struct zone_s *zone_ptr, struct block_s *block_ptr)
{
    struct block_s *tmp_block_ptr = zone_ptr->md_block_head;
    int ret_val = 1;

    while (tmp_block_ptr) {
        if (block_ptr == tmp_block_ptr)
            goto end;
        tmp_block_ptr = tmp_block_ptr->next;
    }
    ret_val = 0;
end:
    return ret_val;
}

int validate_md(struct zone_s **zone_ptr, struct block_s **block_ptr)
{
	int ret_val = 0;
    enum zone_type_e zone_type;
//    struct zone_s *zone_ptr;

    zone_type = MIN_ZONE_TYPE;
    while (zone_type < MAX_ZONE_TYPE) {
        *zone_ptr = malloc_meneger_g.zone_heads[zone_type];
        while (*zone_ptr) {
            if (check_block_ptr(*zone_ptr, *block_ptr))
                goto end;
            *zone_ptr = (*zone_ptr)->next;
        }
        ++zone_type;
    }
    ret_val = -1;
end:
    return (ret_val);
}


static int is_all_blocks_free(struct zone_s *zone_ptr)
{
	int ret_val = 1;
	struct block_s *block_ptr = zone_ptr->md_block_head;

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




static void remove_cur_zone_to_pull(struct zone_s *zone_ptr)
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




void free(void *ptr)
{
    struct zone_s *zone_ptr = NULL;
    struct block_s *block_ptr = NULL;

	pthread_mutex_lock(&mutex_malloc);
	if (ptr == NULL) {
		goto end;
	}
	printf("%s:%d: ptr %p\n", __func__, __LINE__, ptr); //debug
	block_ptr = get_ptr_to_md(ptr);
	printf("%s:%d: ptr %p\n", __func__, __LINE__, ptr); //debug

	if (validate_md(&zone_ptr, &block_ptr)) {
		goto end;
	}
    return_free_block_to_pull(zone_ptr, block_ptr);
	if (is_all_blocks_free(zone_ptr))
		remove_cur_zone_to_pull(zone_ptr);


	printf("%s:%d: bingo! %p\n", __func__, __LINE__, ptr); //debug
end:
	pthread_mutex_unlock(&mutex_malloc);
}
