/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:44:53 by damalca           #+#    #+#             */
/*   Updated: 2025/07/30 15:26:25 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	close_window(t_game *g)
{
	free_game(g);
	exit(1);
}

int	check_args(int ac, char **av)
{
	if (ac != 2)
		return (ft_putstr_fd("Error\nProgram doesn't have 2 args", 2), 0);
	if (!ft_strnstr(av[1], ".ber", ft_strlen(av[1])))
		return (ft_putendl_fd("Error\nYour map isn't at the .ber format", 2), 0);
	return (1);
}
