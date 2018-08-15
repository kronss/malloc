/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_block.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

static void	*create_new_block(struct s_zone *zone_ptr,
									struct s_block *block_ptr, size_t size)
{
	struct s_block	*next_block_ptr;

	next_block_ptr = (struct s_block *)((uint8_t *)block_ptr + size);
	next_block_ptr->alloc_size = block_ptr->alloc_size - size;
	next_block_ptr->free = 1;
	next_block_ptr->next = block_ptr->next;
	next_block_ptr->prev = block_ptr;
	block_ptr->alloc_size = size;
	block_ptr->free = 0;
	block_ptr->next = next_block_ptr;
	zone_ptr->space_left -= size;
	return (block_ptr);
}

/*
** checck current block. Can we insert in it?
*/

static int	check_curr_block(struct s_block *block_ptr, size_t size_request)
{
	int				ret_val;

	ret_val = 0;
	if (size_request < block_ptr->alloc_size)
	{
		ret_val = 1;
	}
	return (ret_val);
}

void		*find_available_block(struct s_zone *zone_ptr, size_t size)
{
	void			*ret_val;
	struct s_block	*block_ptr;

	ret_val = NULL;
	block_ptr = &zone_ptr->md_block_head;
	while (block_ptr)
	{
		if (check_curr_block(block_ptr, size))
		{
			ret_val = create_new_block(zone_ptr, block_ptr, size);
			return (ret_val);
		}
		block_ptr = block_ptr->next;
	}
	return (ret_val);
}
