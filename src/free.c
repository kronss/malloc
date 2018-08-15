/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

void	free(void *ptr)
{
	struct s_zone	*zone_ptr;
	struct s_block	*block_ptr;

	zone_ptr = NULL;
	block_ptr = NULL;
	pthread_mutex_lock(&g_mutex_malloc);
	if (ptr == NULL)
	{
		pthread_mutex_unlock(&g_mutex_malloc);
		return ;
	}
	block_ptr = get_ptr_to_md(ptr);
	if (validate_md(&zone_ptr, &block_ptr))
	{
		pthread_mutex_unlock(&g_mutex_malloc);
		return ;
	}
	free_defragment_unmap(zone_ptr, block_ptr);
	pthread_mutex_unlock(&g_mutex_malloc);
}
