/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 11:12:34 by damalca           #+#    #+#             */
/*   Updated: 2025/08/04 11:25:08 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	init_game(t_game *g)
{
	g->mlx = mlx_init();
	g->win = mlx_new_window(g->mlx, g->w * T_S, g->h * T_S, "so_long");
}

void	init_textures(t_game *g)
{
	g->fl = mlx_xpm_file_to_image(g->mlx, "textures/1.xpm", &g->w, &g->h);
	g->wa = mlx_xpm_file_to_image(g->mlx, "textures/2.xpm", &g->w, &g->h);
	g->col = mlx_xpm_file_to_image(g->mlx, "textures/3.xpm", &g->w, &g->h);
	g->ex = mlx_xpm_file_to_image(g->mlx, "textures/4.xpm", &g->w, &g->h);
	g->pl = mlx_xpm_file_to_image(g->mlx, "textures/5.xpm", &g->w, &g->h);
	if (!g->fl || !g->wa || !g->col || !g->ex || !g->pl)
	{
		free_game(g);
		ft_putstr_fd("Error\nFailed to load textures", 2);
		exit(1);
	}
}

int	init_map(t_game *g, char *file)
{
	g->map = game_map(file);
	if (!g->map)
		return (ft_putstr_fd("Error\nMap couldn't load", 2), 0);
	return (1);
}

void	init_size(t_game *g)
{
	g->h = 0;
	while (g->map[g->h])
		g->h++;
	g->w = ft_strlen(g->map[0]);
}

void	init_game_struct(t_game *g)
{
	g->mlx = NULL;
	g->win = NULL;
	g->map = NULL;
	g->fl = NULL;
	g->wa = NULL;
	g->col = NULL;
	g->ex = NULL;
	g->pl = NULL;
	g->moves = 0;
}
