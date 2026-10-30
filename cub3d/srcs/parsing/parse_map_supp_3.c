/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_supp_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:06:17 by mchiacha          #+#    #+#             */
/*   Updated: 2026/03/13 16:37:25 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	check_map_chars_supp(t_game *game, char c, int x, int y)
{
	c = game->map.grid[y][x];
	if (c != '0' && c != '1' && c != ' '
		&& c != 'N' && c != 'S' && c != 'E' && c != 'W')
		return (ft_error("Invalid character in map"), 0);
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		game->map.player_count++;
		game->player.pos.x = x + 0.5;
		game->player.pos.y = y + 0.5;
		game->player.orientation = c;
		game->map.grid[y][x] = '0';
		init_player_direction(game);
	}
	return (1);
}

int	check_map_chars(t_game *game)
{
	int		x;
	char	c;
	int		y;
	int		line_len;

	game->map.player_count = 0;
	y = -1;
	c = '\0';
	while (++y < game->map.height)
	{
		line_len = ft_strlen(game->map.grid[y]);
		x = -1;
		while (++x < line_len)
		{
			if (check_map_chars_supp(game, c, x, y) == 0)
				return (0);
		}
	}
	if (game->map.player_count != 1)
		return (ft_error("Player count should be 1"), 0);
	return (1);
}

int	is_close_by_walls_supp(int x, int width, int y, t_game *game)
{
	y = game->map.height - 1;
	width = ft_strlen(game->map.grid[y]);
	x = -1;
	while (++x < width)
	{
		if (game->map.grid[y][x] != '1' && game->map.grid[y][x] != ' ')
			return (0);
	}
	y = -1;
	while (++y < game->map.height)
	{
		width = ft_strlen(game->map.grid[y]);
		if (width > 0)
		{
			if (game->map.grid[y][0] != '1' && game->map.grid[y][0] != ' ')
				return (0);
			if (game->map.grid[y][width - 1] != '1'
				&& game->map.grid[y][width - 1] != ' ')
				return (0);
		}
	}
	return (1);
}

int	is_close_by_walls(t_game *game)
{
	int	x;
	int	y;
	int	width;

	y = 0;
	width = ft_strlen(game->map.grid[y]);
	x = -1;
	while (++x < width)
	{
		if (game->map.grid[y][x] != '1' && game->map.grid[y][x] != ' ')
			return (0);
	}
	return (is_close_by_walls_supp(x, width, y, game));
}

int	is_map_fully_closed_supp(t_game *game, int x, int y, int width)
{
	if (game->map.grid[y][x] == '0')
	{
		if (y == 0 || y == game->map.height - 1 || x == 0 || x == width - 1)
			return (0);
		if (x > 0 && game->map.grid[y][x - 1] == ' ')
			return (0);
		if (x < width - 1 && game->map.grid[y][x + 1] == ' ')
			return (0);
		if (y > 0 && x < (int)ft_strlen(game->map.grid[y - 1])
			&& game->map.grid[y - 1][x] == ' ')
			return (0);
		if (y < game->map.height - 1
			&& x < (int)ft_strlen(game->map.grid[y + 1])
			&& game->map.grid[y + 1][x] == ' ')
			return (0);
	}
	return (1);
}
