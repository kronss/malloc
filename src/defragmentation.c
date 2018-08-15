/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defragmentation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

static inline int	is_prev_free(struct s_block *block_ptr)
{
	int		ret_val;

	ret_val = 0;
	if (block_ptr->prev)
	{
		if (block_ptr->prev->free)
		{
			ret_val = 1;
		}
	}
	return (ret_val);
}

static inline int	is_next_free(struct s_block *block_ptr)
{
	int		ret_val;

	ret_val = 0;
	if (block_ptr->next)
	{
		if (block_ptr->next->free)
		{
			ret_val = 1;
		}
	}
	return (ret_val);
}

static inline void	merge_two_free_blocks(struct s_block *a, struct s_block *b)
{
	if (a && b)
	{
		a->next = b->next;
		a->alloc_size += b->alloc_size;
		if (b->next)
		{
			b->next->prev = a;
		}
	}
}

static void			try_defragment(struct s_block *block_ptr)
{
	if (is_next_free(block_ptr))
	{
		merge_two_free_blocks(block_ptr, block_ptr->next);
	}
	if (is_prev_free(block_ptr))
	{
		merge_two_free_blocks(block_ptr->prev, block_ptr);
	}
}

void				free_and_return_block_to_pull(struct s_zone *zone_ptr,
													struct s_block *block_ptr)
{
	block_ptr->free = 1;
	zone_ptr->space_left += block_ptr->alloc_size;
	try_defragment(block_ptr);
}
