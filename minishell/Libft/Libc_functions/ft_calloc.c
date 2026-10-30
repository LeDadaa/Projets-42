/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:47:51 by damalca           #+#    #+#             */
/*   Updated: 2025/04/23 16:28:51 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void			*tab;
	unsigned char	*p;
	int				x;
	size_t			total;

	if (size != 0 && (nmemb > 2147483647 / size))
		return (NULL);
	total = nmemb * size;
	tab = malloc(sizeof(char) * (total));
	if (tab == NULL)
		return (NULL);
	p = (unsigned char *)tab;
	x = 0;
	while (total > 0)
	{
		p[x] = 0;
		x++;
		total--;
	}
	return (tab);
}
