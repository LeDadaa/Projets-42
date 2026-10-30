/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:47:57 by damalca           #+#    #+#             */
/*   Updated: 2025/07/31 11:31:14 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	main(int ac, char **av)
{
	t_game	*g;

	if (!check_args(ac, av))
		return (1);
	g = malloc(sizeof(t_game));
	if (!g)
		return (ft_putstr_fd("Error\nMalloc Failed", 2), 1);
	init_game_struct(g);
	if (!init_map(g, av[1]) || !check_map(g->map))
		return (free_game(g), 1);
	init_size(g);
	init_game(g);
	init_textures(g);
	map_generation(g);
	mlx_key_hook(g->win, handle_moves, g);
	mlx_hook(g->win, DestroyNotify, StructureNotifyMask, close_window, g);
	mlx_loop(g->mlx);
}
