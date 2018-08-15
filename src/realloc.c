/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

void	*realloc(void *ptr, size_t size)
{
	void			*ret_val;
	struct s_block	*block_ptr;
	struct s_zone	*zone_ptr;

	ret_val = NULL;
	zone_ptr = NULL;
	if (ptr == NULL)
		return (malloc(size));
	if (size <= 0)
		return (NULL);
	pthread_mutex_lock(&g_mutex_malloc);
	block_ptr = get_ptr_to_md(ptr);
	if (validate_md(&zone_ptr, &block_ptr))
	{
		pthread_mutex_unlock(&g_mutex_malloc);
		return (NULL);
	}
	if (size < (size_t)(-sizeof(struct s_block)))
	{
		ALIGN_META_INFO(size);
	}
	ret_val = do_realloc(zone_ptr, block_ptr, size);
	ret_val += OFFSETOF(struct s_block, data);
	pthread_mutex_unlock(&g_mutex_malloc);
	return (ret_val);
}
