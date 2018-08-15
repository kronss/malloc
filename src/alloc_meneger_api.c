/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_meneger_api.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

pthread_mutex_t g_mutex_malloc = PTHREAD_MUTEX_INITIALIZER;

struct malloc_meneger_s g_alloc_mnr =
{
	.zone_heads = {NULL, NULL, NULL},
	.print_total_alloc = 0,
};

void	*get_zone_head(size_t size)
{
	void	*ret_val;

	if (size <= TINY_TRESHHOLD)
	{
		ret_val = g_alloc_mnr.zone_heads[TINY];
	}
	else if (size <= SMALL_TRESHHOLD)
	{
		ret_val = g_alloc_mnr.zone_heads[SMALL];
	}
	else
	{
		ret_val = g_alloc_mnr.zone_heads[LARGE];
	}
	return (ret_val);
}

void	*get_ptr_to_md(void *ptr)
{
	void	*ret_val;

	ret_val = ptr - ((size_t)&((struct s_block *)0)->data);
	return (ret_val);
}

int		check_block_ptr(struct s_zone *zone_ptr, struct s_block *block_ptr)
{
	struct s_block	*tmp_block_ptr;
	int				ret_val;

	tmp_block_ptr = &zone_ptr->md_block_head;
	ret_val = 0;
	while (tmp_block_ptr)
	{
		if (block_ptr == tmp_block_ptr)
		{
			if (!block_ptr->free)
			{
				ret_val = 1;
				return (ret_val);
			}
		}
		tmp_block_ptr = tmp_block_ptr->next;
	}
	return (ret_val);
}

int		validate_md(struct s_zone **zone_ptr, struct s_block **block_ptr)
{
	int					ret_val;
	enum e_zone_type	zone_type;

	ret_val = 0;
	zone_type = MIN_ZONE_TYPE;
	while (zone_type < MAX_ZONE_TYPE)
	{
		*zone_ptr = g_alloc_mnr.zone_heads[zone_type];
		while (*zone_ptr)
		{
			if (check_block_ptr(*zone_ptr, *block_ptr))
				return (ret_val);
			*zone_ptr = (*zone_ptr)->next;
		}
		++zone_type;
	}
	ret_val = -1;
	return (ret_val);
}
