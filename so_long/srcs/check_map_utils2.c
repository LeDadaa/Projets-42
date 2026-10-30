/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:18:31 by damalca           #+#    #+#             */
/*   Updated: 2025/07/30 15:06:53 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	check_if_one_line(char **map)
{
	if (!map[1])
		return (1);
	return (0);
}

int	check_if_collectible(char **map)
{
	int	y;
	int	x;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'C')
				return (1);
			x++;
		}
		y++;
	}
	return (0);
}

int	check_if_exit(char **map)
{
	int	y;
	int	x;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'E')
				return (1);
			x++;
		}
		y++;
	}
	return (0);
}

int	check_if_player(char **map)
{
	int	y;
	int	x;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'P')
				return (1);
			x++;
		}
		y++;
	}
	return (0);
}

int	check_if_too_big(char **map)
{
	int		y;
	int		x;
	size_t	width;

	x = 0;
	while (map[0][x])
		x++;
	if (x > 25)
		return (1);
	width = x;
	y = 0;
	while (map[y])
	{
		if (ft_strlen(map[y]) != width)
			return (1);
		y++;
	}
	if (y > 13)
		return (1);
	return (0);
}
