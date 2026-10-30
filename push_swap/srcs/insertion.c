/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insertion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:24:18 by damalca           #+#    #+#             */
/*   Updated: 2025/07/29 16:45:12 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_node	*create_node(int nb)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = nb;
	node->index = 0;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	init_stack_params(t_stack *stack)
{
	stack->head = NULL;
	stack->tail = NULL;
	stack->size = 0;
}

int	init_stack(t_stack *stack, int start, char **av)
{
	int		i;
	int		nb;
	t_node	*new;

	init_stack_params(stack);
	i = start;
	while (av[i] && *av[i])
	{
		nb = ft_atoi(av[i]);
		new = create_node(nb);
		if (!new)
			return (free_stack(stack), 0);
		new->prev = stack->tail;
		if (!stack->head)
			stack->head = new;
		else
			stack->tail->next = new;
		stack->tail = new;
		stack->size++;
		i++;
	}
	if (stack->tail)
		stack->tail->next = NULL;
	return (1);
}
