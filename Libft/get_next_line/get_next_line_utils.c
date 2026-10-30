/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:19:33 by damalca           #+#    #+#             */
/*   Updated: 2025/05/14 15:28:13 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strch(char *s, int c)
{
	int	x;

	if (!s)
		return (0);
	x = 0;
	while (s[x] && s[x] != c)
		x++;
	if (s[x] == c)
		return ((char *)s + x);
	return (NULL);
}

char	*ft_strjoi(char *s1, char *s2)
{
	char	*res;
	int		x;
	int		y;

	if (!s1 || !s2)
		return (NULL);
	res = (char *)malloc(sizeof(char) * (ft_strle(s1) + ft_strle(s2) + 1));
	if (!res)
		return (NULL);
	x = 0;
	while (s1[x])
	{
		res[x] = s1[x];
		x++;
	}
	y = 0;
	while (s2[y])
		res[x++] = s2[y++];
	res[x] = '\0';
	return (res);
}

size_t	ft_strle(char *s)
{
	size_t	x;

	if (!s)
		return (0);
	x = 0;
	while (s[x])
		x++;
	return (x);
}

char	*init_stash(char *stash)
{
	if (stash)
		return (stash);
	stash = malloc(1);
	if (!stash)
		return (NULL);
	stash[0] = '\0';
	return (stash);
}

int	line_len(char *stash)
{
	int	len;

	len = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	if (stash[len] == '\n')
		return (len + 1);
	return (len);
}
