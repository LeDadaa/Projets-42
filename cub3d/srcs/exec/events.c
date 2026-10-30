/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 12:10:04 by damalca           #+#    #+#             */
/*   Updated: 2026/03/16 13:39:22 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <X11/extensions/Xfixes.h>
#include "../../minilibx/mlx_int.h"

void	mlx_mouse_hide_no_leak(void *mlx, void *win)
{
	t_xvar		*xvar;
	t_win_list	*xwin;

	if (mlx == NULL || win == NULL)
		return ;
	xvar = (t_xvar *)mlx;
	xwin = (t_win_list *)win;
	XFixesHideCursor(xvar->display, xwin->window);
}

void	setup_events(t_game *game)
{
	if (!game || !game->mlx || !game->win)
		return ;
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_hook(game->win, 2, 1, key_press, game);
	mlx_hook(game->win, 3, 2, key_release, game);
	mlx_hook(game->win, 6, 1L << 6, mouse_move, game);
	mlx_mouse_hide_no_leak(game->mlx, game->win);
	mlx_mouse_move(game->mlx, game->win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	mlx_loop_hook(game->mlx, game_loop, game);
}
