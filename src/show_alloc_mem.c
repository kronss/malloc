/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochayche <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 20:05:54 by ochayche          #+#    #+#             */
/*   Updated: 2018/08/15 20:06:03 by ochayche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal_api.h"

static const char	*get_zone_type(enum e_zone_type zone_type)
{
	const char *ret_val;

	ret_val = NULL;
	if (zone_type == TINY)
		ret_val = "TINY";
	else if (zone_type == SMALL)
		ret_val = "SMALL";
	else if (zone_type == LARGE)
		ret_val = "LARGE";
	else
		ret_val = "N/A";
	return (ret_val);
}

void				show_meta_block(struct s_block *block_ptr, int verbose)
{
	long	diff;

	while (block_ptr)
	{
		diff = (int8_t *)block_ptr->next - (int8_t *)block_ptr;
		if (!block_ptr->free)
		{
			printf("\t %p - %11p : diff %10ld : %zu bytes\n",
					block_ptr, block_ptr->next, diff, block_ptr->alloc_size);
			g_alloc_mnr.print_total_alloc += diff;
		}
		else if (verbose && block_ptr->free)
		{
			printf("\t %p - %11p : diff %10ld : %zu bytes are freed\n",
					block_ptr, block_ptr->next, diff, block_ptr->alloc_size);
		}
		block_ptr = block_ptr->next;
	}
}

void				show_alloc_zone(enum e_zone_type zone_type, int verbose)
{
	struct s_zone *zone_ptr;

	zone_ptr = g_alloc_mnr.zone_heads[zone_type];
	while (zone_ptr)
	{
		printf("%s : %p\n", get_zone_type(zone_type), zone_ptr);
		show_meta_block(&zone_ptr->md_block_head, verbose);
		zone_ptr = zone_ptr->next;
	}
}

void				show_alloc_mem(void)
{
	enum e_zone_type	zone_type;
	int					verbose;

	verbose = 0;
	pthread_mutex_lock(&g_mutex_malloc);
	g_alloc_mnr.print_total_alloc = 0;
	zone_type = MIN_ZONE_TYPE;
	while (zone_type < MAX_ZONE_TYPE)
	{
		show_alloc_zone(zone_type, verbose);
		++zone_type;
	}
	printf("Total : %zu bytes\n", g_alloc_mnr.print_total_alloc);
	pthread_mutex_unlock(&g_mutex_malloc);
}

void				show_alloc_mem_verbose(void)
{
	enum e_zone_type	zone_type;
	int					verbose;

	verbose = 1;
	pthread_mutex_lock(&g_mutex_malloc);
	g_alloc_mnr.print_total_alloc = 0;
	zone_type = MIN_ZONE_TYPE;
	while (zone_type < MAX_ZONE_TYPE)
	{
		show_alloc_zone(zone_type, verbose);
		++zone_type;
	}
	printf("Total : %zu bytes\n", g_alloc_mnr.print_total_alloc);
	pthread_mutex_unlock(&g_mutex_malloc);
}
