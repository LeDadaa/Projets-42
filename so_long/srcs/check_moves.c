/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_moves.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:48:53 by damalca           #+#    #+#             */
/*   Updated: 2025/06/23 10:12:13 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	move_player(t_game *g, int dx, int dy)
{
	int	x;
	int	y;

	x = g->pl_x + dx;
	y = g->pl_y + dy;
	if (g->map[y][x] == '1')
		return ;
	if (g->map[y][x] == 'C')
	{
		g->c++;
		g->map[y][x] = '0';
	}
	else if (g->map[y][x] == 'E')
	{
		if (g->c == g->t_c)
			close_window(g);
	}
	if (g->map[g->pl_y][g->pl_x] != 'E')
		g->map[g->pl_y][g->pl_x] = '0';
	g->pl_x = x;
	g->pl_y = y;
	g->moves++;
	ft_printf("%d\n", g->moves);
	map_generation(g);
}

int	handle_moves(int keycode, t_game *g)
{
	if (keycode == XK_Escape)
		close_window(g);
	else if (keycode == XK_w)
		move_player(g, 0, -1);
	else if (keycode == XK_s)
		move_player(g, 0, 1);
	else if (keycode == XK_a)
		move_player(g, -1, 0);
	else if (keycode == XK_d)
		move_player(g, 1, 0);
	return (0);
}
