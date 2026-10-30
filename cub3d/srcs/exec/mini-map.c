/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini-map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 11:14:10 by mchiacha          #+#    #+#             */
/*   Updated: 2026/03/13 16:57:57 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_rec(t_game *game, t_vector pos, int size, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size)
		{
			put_pixel_in_img(game, (int)pos.x + i, (int)pos.y + j, color);
			j++;
		}
		i++;
	}
}

static void	draw_mini_player(t_game *game, int start_x, int start_y)
{
	t_vector	player_pos;
	int			size;

	size = MINI_SIZE / 2;
	player_pos.x = start_x + (int)(game->player.pos.x * MINI_SIZE) - size / 2;
	player_pos.y = start_y + (int)(game->player.pos.y * MINI_SIZE) - size / 2;
	draw_rec(game, player_pos, size, 0xFFFFFF);
}

void	draw_mini_map(t_game *game)
{
	t_vector	pos;
	int			x;
	int			y;
	int			color;

	y = -1;
	while (++y < game->map.height)
	{
		x = -1;
		while (game->map.grid[y][++x])
		{
			if (game->map.grid[y][x] == '1')
				color = 0x808080;
			else if (game->map.grid[y][x] == '0')
				color = 0x000000;
			else if (game->map.grid[y][x] == ' ')
				continue ;
			pos.x = 50 + x * MINI_SIZE;
			pos.y = 50 + y * MINI_SIZE;
			draw_rec(game, pos, MINI_SIZE - 1, color);
		}
	}
	draw_mini_player(game, 50, 50);
}
