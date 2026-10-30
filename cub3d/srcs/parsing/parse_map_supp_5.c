/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_supp_5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:19:01 by mchiacha          #+#    #+#             */
/*   Updated: 2026/03/16 11:34:55 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_map_fully_closed(t_game *game)
{
	int	x;
	int	y;
	int	width;

	y = -1;
	while (++y < game->map.height)
	{
		width = ft_strlen(game->map.grid[y]);
		x = -1;
		while (++x < width)
		{
			if (is_map_fully_closed_supp(game, x, y, width) == 0)
				return (0);
		}
	}
	return (1);
}

int	validate_map(t_game *game)
{
	if (!game->map.grid || game->map.height < 3)
		return (ft_error("Map grid NULL or too small"), 0);
	if (!check_map_chars(game))
		return (0);
	if (!is_close_by_walls(game))
		return (ft_error("Map not surrounded by walls"), 0);
	if (!is_map_fully_closed(game))
		return (ft_error("Map has holes or is not closed"), 0);
	return (1);
}

int	parse_rgb(char *line, t_color *color)
{
	char	**rgb;

	rgb = ft_split(line, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
		return (free_split(rgb), 0);
	if (ft_digit(rgb[2]) || ft_digit(rgb[1]) || ft_digit(rgb[0]))
		return (free_split(rgb), ft_error("Error, not digit"), 0);
	color->r = ft_atoi(rgb[0]);
	if (color->r > 255)
		return (free_split(rgb), ft_error("Error, red, 255 for rgb"), 0);
	color->g = ft_atoi(rgb[1]);
	if (color->g > 255)
		return (free_split(rgb), ft_error("Error, green, 255 max for rgb"), 0);
	color->b = ft_atoi(rgb[2]);
	if (color->b > 255)
		return (free_split(rgb), ft_error("Error, blue, 255 for rgb"), 0);
	color->encoded = (color->r << 16) | (color->g << 8) | color->b;
	free_split(rgb);
	return (1);
}

void	normalize_map_to_rectangle_supp(t_game *game, int y, char **new_grid)
{
	int	len;
	int	x;

	new_grid[y] = malloc(game->map.width + 1);
	if (!new_grid[y])
		return ;
	len = ft_strlen(game->map.grid[y]);
	x = -1;
	while (++x < len)
		new_grid[y][x] = game->map.grid[y][x];
	while (x < game->map.width)
	{
		new_grid[y][x] = ' ';
		x++;
	}
	new_grid[y][game->map.width] = '\0';
	free(game->map.grid[y]);
}

void	normalize_map_to_rectangle(t_game *game)
{
	char	**new_grid;
	int		y;
	int		len;

	game->map.width = 0;
	y = -1;
	len = 0;
	while (++y < game->map.height)
	{
		len = ft_strlen(game->map.grid[y]);
		if (len > game->map.width)
			game->map.width = len;
	}
	new_grid = malloc(sizeof(char *) * game->map.height);
	if (!new_grid)
		return ;
	y = -1;
	while (++y < game->map.height)
	{
		normalize_map_to_rectangle_supp(game, y, new_grid);
	}
	free(game->map.grid);
	game->map.grid = new_grid;
}
