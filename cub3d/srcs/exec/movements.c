/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 14:19:47 by damalca           #+#    #+#             */
/*   Updated: 2026/03/16 14:19:52 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	check_wall_collision(t_game *game, double new_x, double new_y)
{
	int	x_sign;
	int	y_sign;
	int	mx;
	int	my;

	y_sign = -1;
	while (y_sign <= 1)
	{
		x_sign = -1;
		while (x_sign <= 1)
		{
			mx = (int)(new_x + x_sign * WALL_MARGIN);
			my = (int)(new_y + y_sign * WALL_MARGIN);
			if (mx < 0 || mx >= game->map.width)
				return (1);
			if (my < 0 || my >= game->map.height)
				return (1);
			if (game->map.grid[my][mx] == '1')
				return (1);
			x_sign += 2;
		}
		y_sign += 2;
	}
	return (0);
}

void	move_forward(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player.pos.x + game->player.dir.x * MOVE_SPEED;
	new_y = game->player.pos.y + game->player.dir.y * MOVE_SPEED;
	if (!check_wall_collision(game, new_x, game->player.pos.y))
		game->player.pos.x = new_x;
	if (!check_wall_collision(game, game->player.pos.x, new_y))
		game->player.pos.y = new_y;
}

void	move_left(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player.pos.x - game->player.plane.x * MOVE_SPEED;
	new_y = game->player.pos.y - game->player.plane.y * MOVE_SPEED;
	if (!check_wall_collision(game, new_x, game->player.pos.y))
		game->player.pos.x = new_x;
	if (!check_wall_collision(game, game->player.pos.x, new_y))
		game->player.pos.y = new_y;
}

void	move_backward(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player.pos.x - game->player.dir.x * MOVE_SPEED;
	new_y = game->player.pos.y - game->player.dir.y * MOVE_SPEED;
	if (!check_wall_collision(game, new_x, game->player.pos.y))
		game->player.pos.x = new_x;
	if (!check_wall_collision(game, game->player.pos.x, new_y))
		game->player.pos.y = new_y;
}

void	move_right(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player.pos.x + game->player.plane.x * MOVE_SPEED;
	new_y = game->player.pos.y + game->player.plane.y * MOVE_SPEED;
	if (!check_wall_collision(game, new_x, game->player.pos.y))
		game->player.pos.x = new_x;
	if (!check_wall_collision(game, game->player.pos.x, new_y))
		game->player.pos.y = new_y;
}
