/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

static void	*get_ptr_from_zone(size_t size, enum e_zone_type zone_type)
{
	void	*ret_val;

	if (!g_alloc_mnr.zone_heads[zone_type])
	{
		g_alloc_mnr.zone_heads[zone_type] = new_zone(zone_type, NULL, size);
		if (!g_alloc_mnr.zone_heads[zone_type])
		{
			ret_val = NULL;
			return (ret_val);
		}
	}
	ret_val = find_zone(size, g_alloc_mnr.zone_heads[zone_type], zone_type);
	return (ret_val);
}

void		*get_ptr(size_t size)
{
	void	*ret_val;

	ret_val = NULL;
	if (size <= TINY_TRESHHOLD)
	{
		ret_val = get_ptr_from_zone(size, TINY);
	}
	else if (size <= SMALL_TRESHHOLD)
	{
		ret_val = get_ptr_from_zone(size, SMALL);
	}
	else
	{
		ret_val = get_ptr_from_zone(size, LARGE);
	}
	return (ret_val);
}

void		*malloc(size_t size)
{
	void	*ret_val;

	pthread_mutex_lock(&g_mutex_malloc);
	ret_val = NULL;
	if (size <= 0)
	{
		pthread_mutex_unlock(&g_mutex_malloc);
		return (ret_val);
	}
	if (size < (size_t)(-sizeof(struct s_block)))
	{
		ALIGN_META_INFO(size);
	}
	ret_val = get_ptr(size);
	ret_val += OFFSETOF(struct s_block, data);
	pthread_mutex_unlock(&g_mutex_malloc);
	return (ret_val);
}
