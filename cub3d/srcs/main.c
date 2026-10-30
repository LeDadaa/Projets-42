/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:15:00 by damalca           #+#    #+#             */
/*   Updated: 2026/03/10 12:47:36 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	check_file_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 5)
		return (0);
	if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (0);
	return (1);
}

static int	check_arguments(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_printf("Error\nUsage: ./cub3d <map.cub>\n");
		return (0);
	}
	if (!check_file_extension(argv[1]))
	{
		ft_printf("Error\nMap file must have .cub extension\n");
		return (0);
	}
	return (1);
}

static int	init_graphics(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (ft_error("Failed to initialize MLX"), 0);
	game->win = mlx_new_window(game->mlx, WIN_WIDTH, WIN_HEIGHT, "cub3D");
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	game->img_addr = mlx_get_data_addr(game->img, &game->bpp,
			&game->line_len, &game->endian);
	load_textures(game);
	return (1);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (!check_arguments(argc, argv))
		return (1);
	init_game(&game);
	if (!parse_file(argv[1], &game))
	{
		ft_error("Failed to parse file");
		free_game(&game);
		return (1);
	}
	if (!validate_map(&game))
	{
		ft_error("Invalid map");
		free_game(&game);
		return (1);
	}
	if (!init_graphics(&game))
		return (free_game(&game), 1);
	init_player(&game);
	mlx_loop_hook(game.mlx, render_frame, &game);
	setup_events(&game);
	mlx_loop(game.mlx);
	free_game(&game);
	return (0);
}
