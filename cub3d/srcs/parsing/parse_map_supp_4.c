/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_supp_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 14:08:17 by mchiacha          #+#    #+#             */
/*   Updated: 2026/03/16 11:44:53 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	parse_texture(char **dest, char *line)
{
	char	**split;

	split = ft_split(line, ' ');
	if (!split || !split[1] || split[2])
		return (free_split(split), 0);
	*dest = ft_strdup(trim_newline(split[1]));
	free_split(split);
	return (1);
}

int	parse_fill_supp_3(int fd, char *line, t_game *game)
{
	int	i;

	line = trim_newline(line);
	if (!line || !*line)
		return (free(line), -1);
	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '1' || line[i] == '0')
	{
		game->map.parsing_map = 1;
		game->map.first_map_line = ft_strdup(line);
		return (free(line), 2);
	}
	else if (!parse_line(line, game))
		return (free(line), close(fd), 0);
	return (free(line), 1);
}

void	parse_fill_supp(t_game *game, int fd)
{
	if (game->map.parsing_map)
	{
		game->map.height = 0;
		game->map.grid = NULL;
		if (game->map.first_map_line)
		{
			game->map.height = 1;
			game->map.grid = malloc(sizeof(char *) * 1);
			if (!game->map.grid)
				return ;
			game->map.grid[0] = game->map.first_map_line;
		}
		read_map_lines(fd, game);
		trim_map_left(game);
		normalize_map_to_rectangle(game);
		calculate_map_width(game);
	}
}

int	parse_file(char *filename, t_game *game)
{
	int		fd;
	char	*line;
	int		ret;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
	game->map.parsing_map = 0;
	line = get_next_line(fd);
	while (!game->map.parsing_map && line != NULL)
	{
		ret = parse_fill_supp_3(fd, line, game);
		if (ret == -1)
		{
			line = get_next_line(fd);
			continue ;
		}
		else if (ret == 2)
			break ;
		else if (ret == 0)
			return (close(fd), 0);
		line = get_next_line(fd);
	}
	return (parse_fill_supp(game, fd), close(fd), 1);
}
