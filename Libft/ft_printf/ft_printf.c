/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:27:00 by damalca           #+#    #+#             */
/*   Updated: 2025/05/05 11:53:22 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	check_format(const char *format, va_list *args)
{
	int	i;

	i = 0;
	if (*format == 'c')
		i += print_char(va_arg(*args, int));
	else if (*format == 's')
		i += print_str(va_arg(*args, char *));
	else if (*format == 'd' || *format == 'i')
		i += print_number(va_arg(*args, int));
	else if (*format == 'u')
		i += print_unsigned_number(va_arg(*args, unsigned int));
	else if (*format == 'x')
		i += print_hexa(va_arg(*args, unsigned int), 'a');
	else if (*format == 'X')
		i += print_hexa(va_arg(*args, unsigned int), 'A');
	else if (*format == 'p')
		i += ptr_to_hexa((unsigned long)va_arg(*args, void *));
	return (i);
}

int	ft_printf(const char *format, ...)
{
	va_list			args;
	unsigned int	len;

	len = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (ft_strchr("cspdiuxX", *format))
				len += check_format(format, &args);
			else if (*format == '%')
				len += print_char('%');
		}
		else
			len += print_char(*format);
		format++;
	}
	va_end(args);
	return (len);
}
