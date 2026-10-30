/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iterative_factorial.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:27:40 by damalca           #+#    #+#             */
/*   Updated: 2025/04/24 09:15:01 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_iterative_factorial(int nb)
{
	int	x;

	if (nb < 0 || nb > 12)
		return (0);
	x = 1;
	while (nb > 1)
		x *= nb--;
	return (x);
}
/*
#include <stdlib.h>
#include <stdio.h>
int main(int ac, char **av)
{
	printf("%d", ft_iterative_factorial(atoi(av[1])));
}*/