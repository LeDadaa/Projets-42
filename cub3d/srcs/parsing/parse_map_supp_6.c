/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_supp_6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 15:34:18 by mchiacha          #+#    #+#             */
/*   Updated: 2026/03/16 11:45:15 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	ft_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		while (str[i] == '\t' || str[i] == ' ')
			i++;
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	parse_line(char *line, t_game *game)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (!line[i])
		return (1);
	if (ft_strncmp(line + i, "NO ", 3) == 0)
		return (parse_texture(&game->config.no_texture, line + i));
	if (ft_strncmp(line + i, "SO ", 3) == 0)
		return (parse_texture(&game->config.so_texture, line + i));
	if (ft_strncmp(line + i, "WE ", 3) == 0)
		return (parse_texture(&game->config.we_texture, line + i));
	if (ft_strncmp(line + i, "EA ", 3) == 0)
		return (parse_texture(&game->config.ea_texture, line + i));
	if (ft_strncmp(line + i, "F ", 2) == 0)
		return (parse_rgb(line + i + 2, &game->config.floor));
	if (ft_strncmp(line + i, "C ", 2) == 0)
		return (parse_rgb(line + i + 2, &game->config.ceiling));
	return (0);
}
