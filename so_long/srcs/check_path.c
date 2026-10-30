/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:28:48 by damalca           #+#    #+#             */
/*   Updated: 2025/06/23 09:10:10 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	flood_fill(char **map, int y, int x)
{
	if (y < 0 || x < 0 || !map[y] || !map[y][x])
		return ;
	if (map[y][x] == '1' || map[y][x] == 'V' || map[y][x] == 'E')
		return ;
	map[y][x] = 'V';
	flood_fill(map, y + 1, x);
	flood_fill(map, y - 1, x);
	flood_fill(map, y, x + 1);
	flood_fill(map, y, x - 1);
}

static char	**map_copy(char **map)
{
	char	**copy;
	int		i;

	i = 0;
	while (map[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (map[i])
	{
		copy[i] = ft_strdup(map[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	find_player(char **map, int *p_y, int *p_x)
{
	*p_y = 0;
	while (map[*p_y])
	{
		*p_x = 0;
		while (map[*p_y][*p_x])
		{
			if (map[*p_y][*p_x] == 'P')
				return ;
			(*p_x)++;
		}
		(*p_y)++;
	}
}

static int	valid_path(char **map)
{
	int	y;
	int	x;
	int	exit_found;
	int	coll_left;

	y = -1;
	coll_left = 0;
	exit_found = 0;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if (map[y][x] == 'C')
				coll_left++;
			else if (map[y][x] == 'E' && ((map[y + 1][x] == 'V') ||
				(map[y - 1][x] == 'V') || (map[y][x - 1] == 'V') ||
				(map[y][x + 1] == 'V')))
				exit_found++;
		}
	}
	if (coll_left != 0 || exit_found != 1)
		return (0);
	return (1);
}

int	check_path(char **map)
{
	char	**copy;
	int		p_y;
	int		p_x;
	int		is_valid;

	copy = map_copy(map);
	if (!copy)
		return (0);
	find_player(map, &p_y, &p_x);
	flood_fill(copy, p_y, p_x);
	is_valid = valid_path(copy);
	p_y = -1;
	while (copy[++p_y])
		free(copy[p_y]);
	free(copy);
	return (is_valid);
}
