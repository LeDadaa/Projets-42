/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:18:14 by damalca           #+#    #+#             */
/*   Updated: 2025/04/24 09:27:51 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_sqrt(int nb)
{
	int	x;

	x = 1;
	while (x * x <= nb)
	{
		if (nb == x * x)
			return (x);
		x++;
	}
	return (0);
}
/*
#include <stdlib.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	printf("%d\n", ft_sqrt(atoi(argv[1])));
}*/