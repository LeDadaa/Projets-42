/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx_and_textures.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 14:19:38 by damalca           #+#    #+#             */
/*   Updated: 2026/03/17 10:19:34 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	load_texture(t_game *game, t_texture *tex, char *path)
{
	int	*bpp;
	int	*line_len;
	int	*endian;
	int	*width;
	int	*height;

	bpp = &tex->bpp;
	line_len = &tex->line_len;
	endian = &tex->endian;
	width = &tex->width;
	height = &tex->height;
	tex->img = mlx_xpm_file_to_image(game->mlx, path, width, height);
	if (!tex->img)
		return (ft_putstr_fd("Image texture failed", 2), 1);
	tex->addr = mlx_get_data_addr(tex->img, bpp, line_len, endian);
	if (!tex->addr)
		return (ft_putstr_fd("Addr init failed", 2), 1);
	return (0);
}

int	init_mlx(t_game *game)
{
	int	*bpp;
	int	*line_len;
	int	*endian;

	bpp = &game->bpp;
	line_len = &game->line_len;
	endian = &game->endian;
	game->mlx = mlx_init();
	if (!game->mlx)
		return (ft_putstr_fd("MLX init failed", 2), 1);
	game->win = mlx_new_window(game->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3d");
	if (!game->win)
		return (ft_putstr_fd("Window init failed", 2), 1);
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!game->img)
		return (ft_putstr_fd("Image init failed", 2), 1);
	game->img_addr = mlx_get_data_addr(game->img, bpp, line_len, endian);
	if (!game->img_addr)
		return (ft_putstr_fd("Image buffer init failed", 2), 1);
	return (0);
}

int	init_textures(t_game *game)
{
	int		i;
	char	*coord[4];

	i = 0;
	coord[0] = game->config.no_texture;
	coord[1] = game->config.so_texture;
	coord[2] = game->config.we_texture;
	coord[3] = game->config.ea_texture;
	while (i < 4)
	{
		if (load_texture(game, &game->textures[i], coord[i]))
			return (1);
		i++;
	}
	return (0);
}
