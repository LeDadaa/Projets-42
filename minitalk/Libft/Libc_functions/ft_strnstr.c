/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:43:58 by damalca           #+#    #+#             */
/*   Updated: 2025/04/28 13:59:27 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	x;
	size_t	y;

	if (!*little)
		return ((char *)big);
	x = 0;
	while (big[x] && x < len)
	{
		y = 0;
		while (big[x + y] == little[y] && (x + y) < len)
		{
			if (little[y + 1] == '\0')
				return ((char *)big + x);
			y++;
		}
		x++;
	}
	return (NULL);
}
/*
int main(int ac, char **av)
{
	printf("%s", strnstr(av[1], av[2], 10));
}*/