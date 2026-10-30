/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:20:50 by damalca           #+#    #+#             */
/*   Updated: 2025/07/29 16:45:22 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	free_tab(char **tab)
{
	int	x;

	x = 0;
	if (!tab)
	{
		return ;
	}
	while (tab[x])
	{
		free(tab[x]);
		tab[x] = NULL;
		x++;
	}
	free(tab);
	tab = NULL;
}

void	free_stack(t_stack *stack)
{
	t_node	*tmp;

	while (stack->head)
	{
		tmp = stack->head->next;
		free(stack->head);
		stack->head = tmp;
	}
	stack->tail = NULL;
	stack->size = 0;
}
