/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:07:05 by damalca           #+#    #+#             */
/*   Updated: 2025/04/23 11:54:29 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	x;
	int	sign;
	int	res;

	x = 0;
	res = 0;
	sign = 1;
	while (nptr[x] == ' ' || (nptr[x] >= 9 && nptr[x] <= 13))
		x++;
	if (nptr[x] == '+' || nptr[x] == '-')
	{
		if (nptr[x] == '-')
			sign = -1;
		x++;
	}
	while (nptr[x] >= '0' && nptr[x] <= '9')
	{
		res = res * 10 + (nptr[x] - 48);
		x++;
	}
	return (res * sign);
}
/*
#include <stdio.h>
int main(int ac, char **av)
{
	printf("%d", ft_atoi(av[1]));
}*/