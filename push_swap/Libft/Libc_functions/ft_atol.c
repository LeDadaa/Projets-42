/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:11:04 by damalca           #+#    #+#             */
/*   Updated: 2025/08/13 14:45:15 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atol(const char *nptr)
{
	int		x;
	int		sign;
	long	res;

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
