#include "malloc_internal_api.h"

void	*new_zone(enum e_zone_type zone_type, struct s_zone *prev_zone, size_t size_request)
{
	void			*raw_ptr;
	struct s_zone	*zone_ptr;
	struct s_block	*block_ptr;
	size_t			size_zone;

	switch (zone_type)
	{
		case TINY:  size_zone = TINY_ZONE  ;	break;
		case SMALL: size_zone = SMALL_ZONE ;	break;
		case LARGE: size_zone = ALIGN_PAGE_SIZE(size_request) ;	break;

		default:	raw_ptr = NULL	;	goto end;
	}

	ALIGN_PAGE_SIZE(size_zone);

	raw_ptr = mmap(NULL, size_zone, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (raw_ptr == MAP_FAILED)
	{
		raw_ptr = NULL;
		goto end;
	}

	zone_ptr = (struct s_zone *)raw_ptr;
	zone_ptr->origin_size = size_zone;
	zone_ptr->space_left = size_zone - sizeof(struct s_zone);
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
	return raw_ptr;
}

void	*find_zone(size_t size_request, struct s_zone *zone_head,
						enum e_zone_type zone_type)
{
	struct s_zone	*zone_ptr;
	void			*ret_val;

	ret_val = NULL;
	zone_ptr = zone_head;
	while (zone_ptr)
	{
		if (size_request < zone_ptr->space_left)
		{
			return (find_available_block(zone_ptr, size_request));
		}
		if (zone_ptr->next)
			zone_ptr = zone_ptr->next;
		else
		{
			zone_ptr->next = new_zone(zone_type, zone_ptr, size_request);
			if (!zone_ptr->next)
			{
				ret_val = NULL;
				break ;
			}
			zone_ptr = zone_ptr->next;
		}
	}
	return (ret_val);
}
