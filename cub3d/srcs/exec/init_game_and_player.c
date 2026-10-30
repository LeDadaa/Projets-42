/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game_and_player.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 14:19:32 by damalca           #+#    #+#             */
/*   Updated: 2026/03/17 10:19:48 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	init_game_supp(t_game *game)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		game->textures[i].img = NULL;
		game->textures[i].addr = NULL;
		game->textures[i].width = 0;
		game->textures[i].height = 0;
		game->textures[i].bpp = 0;
		game->textures[i].line_len = 0;
		game->textures[i].endian = 0;
	}
}

static void	init_default_colors(t_game *game)
{
	game->config.floor.r = -1;
	game->config.floor.g = -1;
	game->config.floor.b = -1;
	game->config.ceiling.r = -1;
	game->config.ceiling.g = -1;
	game->config.ceiling.b = -1;
}

void	init_game(t_game *game)
{
	ft_memset(game, 0, sizeof(*game));
	init_game_supp(game);
	init_default_colors(game);
}

void	init_player_direction(t_game *game)
{
	char	o;

	o = game->player.orientation;
	if (o == 'N' || o == 'S')
	{
		game->player.dir.x = 0;
		game->player.dir.y = -1 + 2 * (o == 'S');
		game->player.plane.x = 0.66 - 1.32 * (o == 'S');
		game->player.plane.y = 0;
	}
	else if (o == 'E' || o == 'W')
	{
		game->player.dir.x = 1 - 2 * (o == 'W');
		game->player.dir.y = 0;
		game->player.plane.x = 0;
		game->player.plane.y = 0.66 - 1.32 * (o == 'W');
	}
}

void	init_player(t_game *game)
{
	int	i;
	int	j;

	i = -1;
	while (++i < game->map.height)
	{
		j = -1;
		while (game->map.grid[i][++j])
		{
			if (ft_strchr("NSEW", game->map.grid[i][j]))
			{
				game->player.pos.x = j + 0.5;
				game->player.pos.y = i + 0.5;
				game->player.orientation = game->map.grid[i][j];
				return ;
			}
		}
	}
}
