/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:58:02 by damalca           #+#    #+#             */
/*   Updated: 2025/04/28 14:22:55 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		x;
	int		y;
	int		len;

	if (!s1 || !set)
		return (NULL);
	x = 0;
	while (s1[x] && ft_strchr(set, s1[x]))
		x++;
	y = ft_strlen(s1) - 1;
	while (y > x && ft_strchr(set, s1[y]))
		y--;
	len = y - x + 1;
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1 + x, len + 1);
	return (res);
}
/*
int	main(int ac, char **av)
{
	if (ac)
		printf("%s", ft_strtrim(av[1], av[2]));
}*/