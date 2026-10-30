/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:36:34 by damalca           #+#    #+#             */
/*   Updated: 2025/04/25 09:45:50 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	*ft_range(int min, int max)
{
	int	*res;
	int	x;

	if (min >= max)
		return (NULL);
	res = (int *)malloc(sizeof(int) * (max - min));
	if (!res)
		return (NULL);
	x = 0;
	while (min < max)
	{
		res[x] = min;
		min++;
		x++;
	}
	return (res);
}
