/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor_and_ceiling.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:40:00 by damalca           #+#    #+#             */
/*   Updated: 2026/03/13 12:09:40 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	fill_row(t_game *game, int y, int color)
{
	int			x;
	char		*dst;

	x = 0;
	while (x < WIN_WIDTH)
	{
		dst = game->img_addr + (y * game->line_len + x * (game->bpp / 8));
		*(unsigned int *)dst = color;
		x++;
	}
}

void	draw_floor_and_ceiling(t_game *game)
{
	int	y;

	y = 0;
	while (y < WIN_HEIGHT / 2)
	{
		fill_row(game, y, game->config.ceiling.encoded);
		y++;
	}
	while (y < WIN_HEIGHT)
	{
		fill_row(game, y, game->config.floor.encoded);
		y++;
	}
}
