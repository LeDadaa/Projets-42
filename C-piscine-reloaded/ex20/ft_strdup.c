/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 10:24:55 by damalca           #+#    #+#             */
/*   Updated: 2025/04/24 12:50:51 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_strlen(char *str)
{
	int	x;

	x = 0;
	while (str[x])
		x++;
	return (x);
}

char	*ft_strdup(char *src)
{
	char	*res;
	int		x;

	res = (char *)malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (res == NULL)
		return (NULL);
	x = 0;
	while (src[x])
	{
		res[x] = src[x];
		x++;
	}
	res[x] = '\0';
	return (res);
}
