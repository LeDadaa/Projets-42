/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_generation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 14:53:03 by damalca           #+#    #+#             */
/*   Updated: 2025/06/23 09:57:29 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	**game_map(char	*file)
{
	int		fd;
	char	*line;
	char	*tmp;
	char	*stock;
	char	**map;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (NULL);
	stock = ft_strdup("");
	line = get_next_line(fd);
	while (line)
	{
		tmp = stock;
		stock = ft_strjoin(stock, line);
		free(tmp);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (!stock)
		return (NULL);
	map = ft_split(stock, '\n');
	free(stock);
	return (map);
}

static void	load_map(t_game *g)
{
	int	x;
	int	y;

	y = 0;
	g->c = 0;
	g->t_c = 0;
	while (g->map[y])
	{
		x = 0;
		while (g->map[y][x])
		{
			if (g->map[y][x] == 'C')
				g->t_c++;
			else if (g->map[y][x] == 'P')
			{
				g->pl_x = x;
				g->pl_y = y;
			}
			x++;
		}
		y++;
	}
}

void	put_img(t_game *g, void *img, int x, int y)
{
	mlx_put_image_to_window(g->mlx, g->win, img, x * T_S, y * T_S);
}

void	map_generation(t_game *g)
{
	int	x;
	int	y;

	load_map(g);
	y = -1;
	while (g->map[++y])
	{
		x = -1;
		while (g->map[y][++x])
		{
			put_img(g, g->fl, x, y);
			if (g->map[y][x] == '1')
				put_img(g, g->wa, x, y);
			else if (g->map[y][x] == 'C')
				put_img(g, g->col, x, y);
			else if (g->map[y][x] == 'E')
				put_img(g, g->ex, x, y);
		}
	}
	put_img(g, g->pl, g->pl_x, g->pl_y);
}
