/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:50:58 by damalca           #+#    #+#             */
/*   Updated: 2025/04/23 16:33:20 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				x;
	const unsigned char	*str;
	unsigned char		d;

	str = (unsigned char *)s;
	d = (unsigned char)c;
	x = 0;
	while (x < n)
	{
		if (str[x] == d)
			return ((void *)(str + x));
		x++;
	}
	return (NULL);
}
