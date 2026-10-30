/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:25:08 by damalca           #+#    #+#             */
/*   Updated: 2025/08/13 15:18:34 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	check_spaces(char *av)
{
	int	x;

	x = 0;
	while (av[x] && av[x] == ' ')
		x++;
	if (av[x] == '\0')
		return (1);
	return (0);
}

void	solve_one(char *av)
{
	char	**tab;
	t_stack	a;
	t_stack	b;

	if (!av || !*av || check_spaces(av))
		(ft_putendl_fd("Error", 2), exit(1));
	tab = ft_split(av, ' ');
	if (!tab)
		(ft_putendl_fd("Error", 2), exit(1));
	check_one(tab);
	if (!init_stack(&a, 0, tab))
	{
		free_tab(tab);
		ft_putendl_fd("Error", 2);
		exit(1);
	}
	free_tab(tab);
	init_stack_params(&b);
	sort(&a, &b);
	free_stack(&a);
	free_stack(&b);
}

void	solve_multiple(int ac, char **av)
{
	t_stack	a;
	t_stack	b;

	check_multiple(ac, av);
	if (!init_stack(&a, 0, av + 1))
	{
		ft_putendl_fd("Error", 2);
		exit(1);
	}
	init_stack_params(&b);
	sort(&a, &b);
	free_stack(&a);
	free_stack(&b);
}
