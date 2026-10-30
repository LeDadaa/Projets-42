/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:57:49 by damalca           #+#    #+#             */
/*   Updated: 2025/04/29 10:37:08 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_char(char c, char sep)
{
	if (c == sep)
		return (1);
	return (0);
}

int	wordcount(char const *str, char sep)
{
	int	count;
	int	x;

	count = 0;
	x = 0;
	while (str[x])
	{
		while (str[x] && is_char(str[x], sep))
			x++;
		if (str[x])
			count++;
		while (str[x] && !is_char(str[x], sep))
			x++;
	}
	return (count);
}

int	wordlen(char const *str, char sep)
{
	int	x;

	x = 0;
	while (str[x] && !(is_char(str[x], sep)))
		x++;
	return (x);
}

void	*ft_free(char **res, size_t x)
{
	while (x > 0)
		free(res[--x]);
	free(res);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		x;
	int		y;
	int		len;

	res = (char **)malloc(sizeof(char *) * (wordcount(s, c) + 1));
	if (!res)
		return (NULL);
	x = 0;
	y = 0;
	while (s[x] && y < wordcount(s, c))
	{
		while (s[x] && is_char(s[x], c))
			x++;
		len = wordlen((s + x), c);
		res[y] = (char *)malloc(sizeof(char) * (len + 1));
		if (!res[y])
			return (ft_free(res, y));
		ft_strlcpy(res[y], (s + x), len + 1);
		y++;
		x += len;
	}
	res[y] = NULL;
	return (res);
}
/*
int	main(int ac, char **av)
{
	char **res;
	if (ac)
	{	
		res = ft_split(av[1], ' ');
		int x = 0;
		while (res[x])
			{
				printf("%s\n", res[x]);
				x++;
			}
	}
}*/