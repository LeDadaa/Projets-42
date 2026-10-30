/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 08:24:00 by damalca           #+#    #+#             */
/*   Updated: 2025/07/31 11:08:01 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	free_map(t_game *g)
{
	int	y;

	if (!g || !g->map)
		return ;
	y = 0;
	while (g->map[y])
	{
		free(g->map[y]);
		y++;
	}
	free(g->map);
	g->map = NULL;
}

static void	free_mlx(t_game *g)
{
	if (!g || !g->mlx)
		return ;
	mlx_destroy_display(g->mlx);
	free(g->mlx);
}

void	free_game(t_game *g)
{
	if (!g)
		return ;
	if (g->pl)
		mlx_destroy_image(g->mlx, g->pl);
	if (g->fl)
		mlx_destroy_image(g->mlx, g->fl);
	if (g->col)
		mlx_destroy_image(g->mlx, g->col);
	if (g->wa)
		mlx_destroy_image(g->mlx, g->wa);
	if (g->ex)
		mlx_destroy_image(g->mlx, g->ex);
	if (g->map)
		free_map(g);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
		free_mlx(g);
	free(g);
}
