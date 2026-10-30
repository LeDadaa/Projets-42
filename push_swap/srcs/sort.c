/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:27:24 by damalca           #+#    #+#             */
/*   Updated: 2025/08/13 14:44:50 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort(t_stack *a, t_stack *b)
{
	if (!a || a->size < 2)
		return ;
	assign_index(a);
	if (is_sorted(a))
		return ;
	if (a->size < 10)
		sort_under_ten(a, b);
	else
		radix_sort(a, b);
}

int	is_sorted(t_stack *a)
{
	t_node	*current;

	current = a->head;
	while (current && current->next)
	{
		if (current->index > current->next->index)
			return (0);
		current = current->next;
	}
	return (1);
}
