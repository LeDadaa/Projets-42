/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:54:22 by damalca           #+#    #+#             */
/*   Updated: 2025/07/30 14:53:08 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	check_map(char **map)
{
	if (!check_if_exists(map))
		return (ft_putstr_fd("Error\nEmpty Map", 2), 0);
	if (!check_chars(map))
		return (ft_putstr_fd("Error\nInvalid Parameters", 2), 0);
	if (check_if_one_line(map) || !check_if_rectangle(map))
		return (ft_putstr_fd("Error\nYour map is not rectangular\n", 2), 0);
	if (!check_walls(map))
		return (ft_putstr_fd("Error\nMap not surrounded by walls", 2), 0);
	if (!check_if_collectible(map))
		return (ft_putstr_fd("Error\nNo collectible in the map", 2), 0);
	if (!check_if_exit(map))
		return (ft_putstr_fd("Error\nNo exit in map", 2), 0);
	if (!check_if_player(map))
		return (ft_putstr_fd("Error\nNo player in map", 2), 0);
	if (check_if_too_big(map))
		return (ft_putstr_fd("Error\nMap too big", 2), 0);
	if (check_duplicates(map))
		return (ft_putstr_fd("Error\nYour map contains duplicates", 2), 0);
	if (!check_path(map))
		return (ft_putstr_fd("Error\nNo valid path in map", 2), 0);
	return (1);
}
