/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printnormal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:27:57 by damalca           #+#    #+#             */
/*   Updated: 2025/05/05 12:25:56 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_char(char c)
{
	write(1, &c, 1);
	return (1);
}

int	print_str(char *str)
{
	unsigned int	x;

	x = 0;
	if (!str)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (str[x])
	{
		ft_putchar(str[x]);
		x++;
	}
	return (x);
}

int	print_number(int n)
{
	unsigned int	nb;
	unsigned int	x;

	x = 0;
	if (n < 0)
	{
		x += print_char('-');
		nb = (unsigned int)(-(long)n);
	}
	else
		nb = n;
	if (nb >= 10)
		x += print_number(nb / 10);
	x += print_char((nb % 10) + '0');
	return (x);
}

int	print_unsigned_number(unsigned int n)
{
	unsigned int	x;

	x = 0;
	if (n >= 10)
		x += print_unsigned_number(n / 10);
	x += print_char((n % 10) + '0');
	return (x);
}
