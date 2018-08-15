/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

static void	init_zone_headers(struct s_zone *zone_ptr, struct s_zone *prev_zone,
							size_t size_zone)
{
	struct s_block	*block_ptr;

	zone_ptr->origin_size = size_zone;
	zone_ptr->space_left = size_zone - sizeof(struct s_zone);
	zone_ptr->next = NULL;
	zone_ptr->prev = prev_zone;
	if (prev_zone)
		prev_zone->next = zone_ptr;
	block_ptr = &(zone_ptr->md_block_head);
	block_ptr->next = NULL;
	block_ptr->prev = NULL;
	block_ptr->alloc_size = zone_ptr->space_left;
	block_ptr->free = 1;
}

void		*new_zone(enum e_zone_type zone_type, struct s_zone *prev_zone,
					size_t size_request)
{
	void			*raw_ptr;
	size_t			size_zone;

	if (zone_type == TINY)
		size_zone = TINY_ZONE;
	else if (zone_type == SMALL)
		size_zone = SMALL_ZONE;
	else
		size_zone = ALIGN_PAGE_SIZE(size_request);
	raw_ptr = mmap(NULL, size_zone, PROT_FLAGS, MAP_FLAGS, -1, 0);
	if (raw_ptr == MAP_FAILED)
		raw_ptr = NULL;
	else
	{
		init_zone_headers((struct s_zone *)raw_ptr, prev_zone, size_zone);
	}
	return (raw_ptr);
}

void		*find_zone(size_t size_request, struct s_zone *zone_head,
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
