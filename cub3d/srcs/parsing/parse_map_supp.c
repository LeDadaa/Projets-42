/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_supp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:10:59 by mchiacha          #+#    #+#             */
/*   Updated: 2026/03/13 13:54:49 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	load_textures(t_game *game)
{
	int		i;
	char	*paths[4];

	paths[0] = game->config.no_texture;
	paths[1] = game->config.so_texture;
	paths[2] = game->config.we_texture;
	paths[3] = game->config.ea_texture;
	i = -1;
	while (++i < 4)
	{
		game->textures[i].img = mlx_xpm_file_to_image(game->mlx, paths[i],
				&game->textures[i].width, &game->textures[i].height);
		if (!game->textures[i].img)
		{
			ft_printf("Error\nCannot load texture: %s\n", paths[i]);
			continue ;
		}
		game->textures[i].addr = mlx_get_data_addr(game->textures[i].img,
				&game->textures[i].bpp, &game->textures[i].line_len,
				&game->textures[i].endian);
	}
	game->config.textures_loaded = 1;
}

void	free_game(t_game *game)
{
	int	i;

	free(game->config.no_texture);
	free(game->config.so_texture);
	free(game->config.we_texture);
	free(game->config.ea_texture);
	if (game->map.grid)
	{
		i = -1;
		while (++i < game->map.height)
			free(game->map.grid[i]);
		free(game->map.grid);
	}
	i = -1;
	while (++i < 4)
	{
		if (game->textures[i].img)
			mlx_destroy_image(game->mlx, game->textures[i].img);
	}
	get_next_line(-1);
}

void	ft_error(char *msg)
{
	ft_printf("Error\n%s\n", msg);
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
}

char	*trim_newline(char *str)
{
	int	len;

	len = ft_strlen(str);
	if (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
		str[len - 1] = '\0';
	if (len > 1 && (str[len - 2] == '\r'))
		str[len - 2] = '\0';
	return (str);
}
