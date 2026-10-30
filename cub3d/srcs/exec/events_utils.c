/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:57:57 by damalca           #+#    #+#             */
/*   Updated: 2026/03/13 11:58:29 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	close_window(t_game *game)
{
	free_game(game);
	if (game->img)
		mlx_destroy_image(game->mlx, game->img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	ft_memset(game, 0, sizeof(*game));
	exit(0);
}

int	key_press(int keycode, t_game *game)
{
	if (keycode == ESC)
		close_window(game);
	else if (keycode == W)
		game->keys.w = 1;
	else if (keycode == A)
		game->keys.a = 1;
	else if (keycode == S)
		game->keys.s = 1;
	else if (keycode == D)
		game->keys.d = 1;
	else if (keycode == LEFT)
		game->keys.left = 1;
	else if (keycode == RIGHT)
		game->keys.right = 1;
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == W)
		game->keys.w = 0;
	else if (keycode == A)
		game->keys.a = 0;
	else if (keycode == S)
		game->keys.s = 0;
	else if (keycode == D)
		game->keys.d = 0;
	else if (keycode == LEFT)
		game->keys.left = 0;
	else if (keycode == RIGHT)
		game->keys.right = 0;
	return (0);
}

int	mouse_move(int x, int y, t_game *game)
{
	int	delta;

	(void)y;
	if (game->mouse_warping)
	{
		game->mouse_warping = 0;
		return (0);
	}
	delta = x - WIN_WIDTH / 2;
	if (delta == 0)
		return (0);
	rotate_by_angle(game, delta * MOUSE_SENSITIVITY);
	game->mouse_warping = 1;
	mlx_mouse_move(game->mlx, game->win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	return (0);
}

int	game_loop(t_game *game)
{
	if (game->keys.w)
		move_forward(game);
	if (game->keys.s)
		move_backward(game);
	if (game->keys.a)
		move_left(game);
	if (game->keys.d)
		move_right(game);
	if (game->keys.left)
		rotate_left(game);
	if (game->keys.right)
		rotate_right(game);
	render_frame(game);
	return (0);
}
