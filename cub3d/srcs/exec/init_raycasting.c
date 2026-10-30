/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycasting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 12:33:23 by damalca           #+#    #+#             */
/*   Updated: 2026/03/13 17:09:49 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	render_frame(t_game *game)
{
	int	x;

	x = 0;
	draw_floor_and_ceiling(game);
	while (x < WIN_WIDTH)
	{
		cast_ray(game, x);
		x++;
	}
	if (game->map.height < 50 && game->map.width < 70)
		draw_mini_map(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
	return (0);
}

void	cast_ray(t_game *game, int x)
{
	t_ray	ray;

	init_ray(&ray, game, x);
	algo_dda(&ray, game);
	wall_height(&ray);
	render_column(game, &ray, x);
}
