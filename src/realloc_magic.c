/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_magic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

static int	need_realloc_in_same_zone(struct s_block *block_ptr,
										size_t new_size)
{
	int		ret_val;

	ret_val = 0;
	if (get_zone_head(block_ptr->alloc_size) == get_zone_head(new_size))
	{
		ret_val = 1;
	}
	return (ret_val);
}

static int	can_be_expanded(struct s_block *block_ptr, size_t new_size)
{
	int				ret_val;
	struct s_block	*next_block_ptr;

	ret_val = 0;
	next_block_ptr = block_ptr->next;
	if (!next_block_ptr || !next_block_ptr->free)
	{
		if (new_size <= (block_ptr->alloc_size + next_block_ptr->alloc_size
												- METABLOCK_SIZE))
		{
			ret_val = 1;
		}
	}
	return (ret_val);
}

static void	*expand_curr_block(struct s_zone *zone_ptr,
							struct s_block *block_ptr, size_t new_size)
{
	struct s_block	*old_next_block_ptr;
	struct s_block	*next_block_ptr;

	old_next_block_ptr = block_ptr->next;
	next_block_ptr = NULL;
	zone_ptr->space_left += block_ptr->alloc_size;
	next_block_ptr = (struct s_block *)((uint8_t *)block_ptr + new_size);
	next_block_ptr->alloc_size = old_next_block_ptr->alloc_size -
								(new_size - block_ptr->alloc_size);
	next_block_ptr->free = 1;
	next_block_ptr->next = old_next_block_ptr->next;
	next_block_ptr->prev = block_ptr;
	block_ptr->alloc_size = new_size;
	block_ptr->free = 0;
	block_ptr->next = next_block_ptr;
	zone_ptr->space_left -= block_ptr->alloc_size;
	return (block_ptr);
}

static void	*do_realloc_in_same_zone(struct s_zone *zone_ptr,
							struct s_block *old_block_ptr, size_t new_size)
{
	struct s_block	*new_block_ptr;

	new_block_ptr = NULL;
	if (can_be_expanded(old_block_ptr, new_size))
	{
		new_block_ptr = expand_curr_block(zone_ptr, old_block_ptr, new_size);
	}
	else
	{
		new_block_ptr = get_ptr(new_size);
		ft_memmove(new_block_ptr->data, old_block_ptr->data,
					old_block_ptr->alloc_size - OFFSETOF(struct s_block, data));
		free_defragment_unmap(zone_ptr, old_block_ptr);
	}
	return (void *)new_block_ptr;
}

void		*do_realloc(struct s_zone *zone_ptr, struct s_block *block_ptr,
					size_t new_size)
{
	struct s_block	*new_block_ptr;

	new_block_ptr = NULL;
	if (need_realloc_in_same_zone(block_ptr, new_size))
	{
		new_block_ptr = do_realloc_in_same_zone(zone_ptr, block_ptr, new_size);
	}
	else
	{
		new_block_ptr = get_ptr(new_size);
		ft_memmove(new_block_ptr->data, block_ptr->data,
			block_ptr->alloc_size - OFFSETOF(struct s_block, data));
		free_defragment_unmap(zone_ptr, block_ptr);
	}
	return (void *)new_block_ptr;
}
