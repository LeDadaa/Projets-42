/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo_dda.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 14:19:17 by damalca           #+#    #+#             */
/*   Updated: 2026/03/17 10:22:40 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	step_ray(t_ray *ray)
{
	if (ray->side_dist.x < ray->side_dist.y)
	{
		ray->side_dist.x += ray->delta_dist.x;
		ray->map.x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist.y += ray->delta_dist.y;
		ray->map.y += ray->step_y;
		ray->side = 1;
	}
}

static void	set_perp_wall_dist(t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = fabs(ray->side_dist.x - ray->delta_dist.x);
	else
		ray->perp_wall_dist = fabs(ray->side_dist.y - ray->delta_dist.y);
}

static int	is_outside_map(t_ray *ray, t_game *game)
{
	if (ray->map.x < 0 || ray->map.x >= game->map.width)
		return (1);
	if (ray->map.y < 0 || ray->map.y >= game->map.height)
		return (1);
	return (0);
}

void	algo_dda(t_ray *ray, t_game *game)
{
	int	hit_wall;

	hit_wall = 0;
	while (!hit_wall)
	{
		step_ray(ray);
		if (is_outside_map(ray, game))
			break ;
		if (game->map.grid[(int) ray->map.y][(int) ray->map.x] == '1')
			hit_wall = 1;
	}
	set_perp_wall_dist(ray);
}
