/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:33:35 by damalca           #+#    #+#             */
/*   Updated: 2025/04/23 16:40:57 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dl;
	size_t	sl;
	size_t	x;

	dl = ft_strlen(dst);
	sl = ft_strlen(src);
	if (dl >= size)
		return (size + sl);
	x = 0;
	while (src[x] && (dl + x) < (size - 1))
	{
		dst[dl + x] = src[x];
		x++;
	}
	dst[dl + x] = '\0';
	return (dl + sl);
}
