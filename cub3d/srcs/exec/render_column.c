/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 11:31:07 by damalca           #+#    #+#             */
/*   Updated: 2026/03/13 12:09:40 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	put_pixel_in_img(t_game *game, int x, int y, int color)
{
	char	*pixel;

	if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
		return ;
	pixel = game->img_addr + (y * game->line_len + x * (game->bpp / 8));
	*(unsigned int *)pixel = color;
}

static int	get_texture_index(t_ray *ray)
{
	if (ray->side == 0 && ray->dir.x > 0)
		return (3);
	if (ray->side == 0 && ray->dir.x <= 0)
		return (2);
	if (ray->dir.y > 0)
		return (1);
	return (0);
}

static int	get_tex_color(t_texture *tex, int tex_x, int tex_y)
{
	char	*pixel;

	if (tex_x < 0 || tex_y < 0 || tex_x >= tex->width || tex_y >= tex->height)
		return (0);
	pixel = tex->addr + (tex_y * tex->line_len + tex_x * (tex->bpp / 8));
	return (*(unsigned int *)pixel);
}

static void	draw_wall(t_game *game, t_ray *ray, int x, t_texture *tex)
{
	int		y;
	int		tex_y;
	double	step;
	double	tex_pos;

	step = (double)TEX_SIZE / ray->line_height;
	tex_pos = (ray->draw_start - WIN_HEIGHT / 2 + ray->line_height / 2) * step;
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		tex_y = (int)tex_pos & (TEX_SIZE - 1);
		put_pixel_in_img(game, x, y, get_tex_color(tex, ray->tex_x, tex_y));
		tex_pos += step;
		y++;
	}
}

void	render_column(t_game *game, t_ray *ray, int x)
{
	int	tex_index;

	if (ray->side == 0)
		ray->wall_x = game->player.pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		ray->wall_x = game->player.pos.x + ray->perp_wall_dist * ray->dir.x;
	ray->wall_x -= floor(ray->wall_x);
	ray->tex_x = (int)(ray->wall_x * (double)TEX_SIZE);
	if ((ray->side == 0 && ray->dir.x > 0)
		|| (ray->side == 1 && ray->dir.y < 0))
		ray->tex_x = TEX_SIZE - ray->tex_x - 1;
	tex_index = get_texture_index(ray);
	draw_wall(game, ray, x, &game->textures[tex_index]);
}
