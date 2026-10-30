/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 11:49:28 by damalca           #+#    #+#             */
/*   Updated: 2026/03/16 11:47:26 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	read_map_lines_supp(t_game *game, int ch, char **ng, char *cl)
{
	ng[ch - 1] = cl;
	if (game->map.grid)
		free(game->map.grid);
	game->map.grid = ng;
	game->map.height = ch;
}

static void	read_map_lines_supp_2(int *current_height, char *line, int *i)
{
	free(line);
	*current_height += 1;
	*i = -1;
}

int	read_map_lines(int fd, t_game *game)
{
	char	*line;
	char	*clean_line;
	char	**new_grid;
	int		i;
	int		current_height;

	current_height = game->map.height;
	line = get_next_line(fd);
	while (line != NULL)
	{
		line = trim_newline(line);
		clean_line = ft_strdup(line);
		if (!clean_line)
			continue ;
		read_map_lines_supp_2(&current_height, line, &i);
		new_grid = malloc(sizeof(char *) * current_height);
		if (!new_grid)
			return (0);
		while (++i < game->map.height)
			new_grid[i] = game->map.grid[i];
		read_map_lines_supp(game, current_height, new_grid, clean_line);
		line = get_next_line(fd);
	}
	return (1);
}

void	calculate_map_width(t_game *game)
{
	int	y;
	int	len;

	game->map.width = 0;
	y = -1;
	while (++y < game->map.height)
	{
		len = ft_strlen(game->map.grid[y]);
		if (len > game->map.width)
			game->map.width = len;
	}
}

void	trim_map_left(t_game *game)
{
	int		y;
	int		x;
	int		min_start;
	char	*new_line;

	min_start = 1000000;
	y = -1;
	while (++y < game->map.height)
	{
		x = 0;
		while (game->map.grid[y][x]
			&& (game->map.grid[y][x] == ' ' || game->map.grid[y][x] == '\t'))
			x++;
		if (x < min_start)
			min_start = x;
	}
	y = -1;
	while (++y < game->map.height)
	{
		new_line = ft_strdup(game->map.grid[y] + min_start);
		free(game->map.grid[y]);
		game->map.grid[y] = new_line;
	}
}
