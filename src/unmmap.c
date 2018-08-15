/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unmmap.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

static int	is_all_blocks_free(struct s_zone *zone_ptr)
{
	int				ret_val;
	struct s_block	*block_ptr;

	ret_val = 1;
	block_ptr = &zone_ptr->md_block_head;
	while (block_ptr)
	{
		if (block_ptr->free == 0)
		{
			ret_val = 0;
			break ;
		}
		block_ptr = block_ptr->next;
	}
	return (ret_val);
}

static void	remove_cur_zone_to_pull(struct s_zone *zone_ptr)
{
	enum e_zone_type	zone_type;
	struct s_zone		*zone_next;
	struct s_zone		*zone_prev;

	zone_type = MIN_ZONE_TYPE;
	zone_next = zone_ptr->next;
	zone_prev = zone_ptr->prev;
	if (zone_prev)
		zone_prev->next = zone_next;
	else
	{
		while (zone_type < MAX_ZONE_TYPE)
		{
			if (g_alloc_mnr.zone_heads[zone_type] == zone_ptr)
			{
				g_alloc_mnr.zone_heads[zone_type] = zone_next;
			}
			++zone_type;
		}
	}
	if (zone_prev)
		zone_prev->prev = zone_prev;
	munmap(zone_ptr, zone_ptr->origin_size);
}

static int	last_zone(struct s_zone *zone_head)
{
	int		ret_val;

	ret_val = 0;
	if (zone_head)
	{
		if (!zone_head->next && !zone_head->prev)
		{
			ret_val = 1;
		}
	}
	return (ret_val);
}

void		free_defragment_unmap(struct s_zone *zone_ptr,
									struct s_block *block_ptr)
{
	struct s_zone *zone_head;

	zone_head = get_zone_head(block_ptr->alloc_size);
	free_and_return_block_to_pull(zone_ptr, block_ptr);
	if (is_all_blocks_free(zone_ptr) && !last_zone(zone_head))
	{
		remove_cur_zone_to_pull(zone_ptr);
	}
}
