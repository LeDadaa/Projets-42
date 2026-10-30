/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:01:56 by damalca           #+#    #+#             */
/*   Updated: 2025/04/28 09:28:10 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;
	size_t	x;

	len = 0;
	while (src[len])
		len++;
	if (size == 0)
		return (len);
	x = 0;
	while (x < size - 1 && src[x])
	{
		dst[x] = src[x];
		x++;
	}
	dst[x] = '\0';
	return (len);
}
/*
#include <stdio.h>
int main()
{
	char s[] = "abcde";
	char d[] = "123";
	printf("%zu\n", ft_strlcpy(d, s, 1));
	ft_strlcpy(d, s, 1);
	printf("%s", d);
}*/