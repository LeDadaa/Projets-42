/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:25:31 by damalca           #+#    #+#             */
/*   Updated: 2025/08/13 13:30:21 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	check_for_numbers(char *str)
{
	int	x;

	x = 0;
	if (str[x] == '-')
		x++;
	while (str[x])
	{
		if (!ft_isdigit(str[x]))
			return (0);
		x++;
	}
	return (1);
}

static int	check_if_int(char *tab)
{
	long	nb;

	nb = ft_atol(tab);
	if (nb < INT_MIN || nb > INT_MAX)
		return (0);
	return (1);
}

static int	check_duplicates(char **tab)
{
	int	x;
	int	y;

	if (!tab)
		return (1);
	x = 0;
	while (tab[x])
	{
		y = x + 1;
		if (!check_for_numbers(tab[x]) || !check_if_int(tab[x]))
			return (1);
		while (tab[y])
		{
			if (ft_atoi(tab[x]) == ft_atoi(tab[y]))
				return (1);
			y++;
		}
		x++;
	}
	return (0);
}

void	check_one(char **tab)
{
	if (check_duplicates(tab))
	{
		free_tab(tab);
		ft_putendl_fd("Error", 2);
		exit(1);
	}
}

void	check_multiple(int ac, char **av)
{
	int	x;
	int	y;

	x = 1;
	while (x < ac)
	{
		y = x + 1;
		if (!check_for_numbers(av[x]) || !check_if_int(av[x]))
		{
			ft_putendl_fd("Error", 2);
			exit(1);
		}
		while (y < ac)
		{
			if (ft_atoi(av[x]) == ft_atoi(av[y]))
			{
				ft_putendl_fd("Error", 2);
				exit(1);
			}
			y++;
		}
		x++;
	}
}
