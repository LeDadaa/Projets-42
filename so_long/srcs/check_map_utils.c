/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:48:12 by damalca           #+#    #+#             */
/*   Updated: 2025/06/23 09:08:04 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	check_if_exists(char **map)
{
	if (!map || !map[0])
		return (0);
	return (1);
}

int	check_if_rectangle(char **map)
{
	int	y;

	y = 0;
	while (map[++y])
	{
		if (ft_strlen(map[0]) != ft_strlen(map[y]))
			return (0);
	}
	return (1);
}

int	check_duplicates(char **map)
{
	int	exit_count;
	int	player_count;
	int	y;
	int	x;

	exit_count = 0;
	player_count = 0;
	y = -1;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if (map[y][x] == 'E')
				exit_count++;
			else if (map[y][x] == 'P')
				player_count++;
		}
	}
	if (exit_count != 1 || player_count != 1)
		return (1);
	return (0);
}

int	check_chars(char **map)
{
	int	y;
	int	x;

	y = -1;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if (map[y][x] != '1' && map[y][x] != '0' &&
				map[y][x] != 'C' && map[y][x] != 'P' &&
				map[y][x] != 'E')
				return (0);
		}
	}
	return (1);
}

int	check_walls(char **map)
{
	int	y;
	int	x;
	int	width;

	width = ft_strlen(map[0]);
	x = -1;
	while (map[0][++x])
	{
		if (map[0][x] != '1')
			return (0);
	}
	y = 1;
	while (map[y + 1])
	{
		if (map[y][0] != '1' || map[y][width - 1] != '1')
			return (0);
		y++;
	}
	x = -1;
	while (map[y][++x])
	{
		if (map[y][x] != '1')
			return (0);
	}
	return (1);
}
