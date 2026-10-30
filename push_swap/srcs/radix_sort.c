/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   radix_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:27:05 by damalca           #+#    #+#             */
/*   Updated: 2025/07/29 16:53:38 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	assign_index(t_stack *a)
{
	t_node	*current;
	t_node	*comp;
	int		index;

	current = a->head;
	while (current)
	{
		index = 0;
		comp = a->head;
		while (comp)
		{
			if (comp->value < current->value)
				index++;
			comp = comp->next;
		}
		current->index = index;
		current = current->next;
	}	
}

static size_t	get_max_bits(int size)
{
	size_t	bits;

	bits = 0;
	if (size == 0)
		return (1);
	while (size)
	{
		size >>= 1;
		bits++;
	}
	return (bits);
}

void	radix_sort(t_stack *a, t_stack *b)
{
	size_t	max_bits;
	size_t	i;
	size_t	j;

	if (!a || !b || a->size < 2)
		return ;
	max_bits = get_max_bits(a->size - 1);
	i = 0;
	while (i < max_bits)
	{
		j = a->size;
		while (j > 0)
		{
			if ((a->head->index >> i) & 1)
				ra(a);
			else
				pb(a, b);
			j--;
		}
		while (b->size > 0)
			pa(a, b);
		i++;
	}
}
