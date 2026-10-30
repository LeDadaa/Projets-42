/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_params.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:44:14 by damalca           #+#    #+#             */
/*   Updated: 2025/04/24 12:49:54 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_putchar(char c);

int	main(int ac, char **av)
{
	int	x;
	int	y;

	x = 1;
	while (x < ac)
	{
		y = 0;
		while (av[x][y])
			ft_putchar(av[x][y++]);
		ft_putchar('\n');
		x++;
	}
}
