/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_small.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:28:06 by damalca           #+#    #+#             */
/*   Updated: 2025/08/14 10:01:53 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	find_min(t_stack *stack)
{
	t_node	*node;
	int		min;

	node = stack->head;
	min = node->index;
	while (node)
	{
		if (node->index < min)
			min = node->index;
		node = node->next;
	}
	return (min);
}

static size_t	get_distance_to_min(t_stack *a)
{
	t_node	*current;
	int		min;
	size_t	distance;

	min = find_min(a);
	current = a->head;
	distance = 0;
	while (current && current->index != min)
	{
		current = current->next;
		distance++;
	}
	return (distance);
}

static void	sort_three(t_stack *a)
{
	int	first;
	int	second;
	int	third;

	first = a->head->index;
	second = a->head->next->index;
	third = a->head->next->next->index;
	if (first > second && second < third && first < third)
		sa(a);
	else if (first > second && second > third)
	{
		sa(a);
		rra(a);
	}
	else if (first > second && second < third && first > third)
		ra(a);
	else if (first < second && second > third && first < third)
	{
		sa(a);
		ra(a);
	}
	else if (first < second && second > third && first > third)
		rra(a);
}

static void	sort_under_four(t_stack *a)
{
	if (a->size == 2)
	{
		if (a->head->index > a->head->next->index)
			sa(a);
		return ;
	}
	else if (a->size == 3)
		sort_three(a);
}

void	sort_under_ten(t_stack *a, t_stack *b)
{
	int		min_index;
	size_t	distance;

	while (a->size > 3)
	{
		min_index = find_min(a);
		distance = get_distance_to_min(a);
		if (distance <= a->size / 2)
			while (a->head->index != min_index)
				ra(a);
		else
			while (a->head->index != min_index)
				rra(a);
		if (is_sorted(a))
			break ;
		pb(a, b);
	}
	sort_under_four(a);
	while (b->size > 0)
		pa(a, b);
}
