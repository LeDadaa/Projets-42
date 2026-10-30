/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printhexa.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:11:41 by damalca           #+#    #+#             */
/*   Updated: 2025/05/05 11:54:17 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_hexa(unsigned int n, char Uplo)
{
	int	x;

	x = 0;
	if (n >= 16)
		x += print_hexa(n / 16, Uplo);
	if ((n % 16) < 10)
		x += print_char('0' + (n % 16));
	else
		x += print_char(Uplo + (n % 16 - 10));
	return (x);
}

int	print_ptr(unsigned long n)
{
	int	x;

	x = 0;
	if (n >= 16)
		x += print_ptr(n / 16);
	if ((n % 16) < 10)
		x += print_char('0' + (n % 16));
	else
		x += print_char('a' + (n % 16 - 10));
	return (x);
}

int	ptr_to_hexa(unsigned long ptr)
{
	if (!ptr)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	write(1, "0x", 2);
	return ((print_ptr((unsigned long)ptr) + 2));
}
