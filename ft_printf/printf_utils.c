/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:12:38 by damalca           #+#    #+#             */
/*   Updated: 2025/05/05 10:42:51 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

char	*ft_strchr(const char *s, int c)
{
	int	x;

	x = 0;
	while (s[x] && s[x] != (char)c)
		x++;
	if (s[x] == (char)c)
		return ((char *)s + x);
	return (NULL);
}
