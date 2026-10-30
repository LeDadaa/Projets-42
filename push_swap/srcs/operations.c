/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:24:49 by damalca           #+#    #+#             */
/*   Updated: 2025/08/13 14:42:44 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sa(t_stack *a)
{
	t_node	*first;
	t_node	*second;
	int		tmp_value;
	int		tmp_index;

	if (!a || a->size < 2)
		return ;
	first = a->head;
	second = first->next;
	tmp_value = first->value;
	tmp_index = first->index;
	first->value = second->value;
	first->index = second->index;
	second->value = tmp_value;
	second->index = tmp_index;
	ft_putendl_fd("sa", 1);
}

void	ra(t_stack *a)
{
	t_node	*first;

	if (!a || a->size < 2)
		return ;
	first = a->head;
	a->head = first->next;
	a->head->prev = NULL;
	a->tail->next = first;
	first->prev = a->tail;
	first->next = NULL;
	a->tail = first;
	ft_putendl_fd("ra", 1);
}

void	rra(t_stack *a)
{
	t_node	*last;

	if (!a || a->size < 2)
		return ;
	last = a->tail;
	a->tail = last->prev;
	a->tail->next = NULL;
	last->next = a->head;
	a->head->prev = last;
	last->prev = NULL;
	a->head = last;
	ft_putendl_fd("rra", 1);
}

void	pb(t_stack *a, t_stack *b)
{
	t_node	*temp;

	if (!a || a->size == 0)
		return ;
	temp = a->head;
	a->head = temp->next;
	if (a->head)
		a->head->prev = NULL;
	a->size--;
	if (a->size == 0)
		a->tail = NULL;
	temp->next = b->head;
	if (b->head)
		b->head->prev = temp;
	b->head = temp;
	temp->prev = NULL;
	if (b->size == 0)
		b->tail = temp;
	b->size++;
	ft_putendl_fd("pb", 1);
}

void	pa(t_stack *a, t_stack *b)
{
	t_node	*temp;

	if (!b || b->size == 0)
		return ;
	temp = b->head;
	b->head = temp->next;
	if (b->head)
		b->head->prev = NULL;
	b->size--;
	if (b->size == 0)
		b->tail = NULL;
	temp->next = a->head;
	if (a->head)
		a->head->prev = temp;
	a->head = temp;
	temp->prev = NULL;
	if (a->size == 0)
		a->tail = temp;
	a->size++;
	ft_putendl_fd("pa", 1);
}
