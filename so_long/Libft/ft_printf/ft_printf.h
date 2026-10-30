/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:49:32 by damalca           #+#    #+#             */
/*   Updated: 2025/05/05 12:26:02 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int		ft_printf(const char *format, ...);
int		print_char(char c);
int		print_str(char *str);
int		print_number(int n);
int		print_unsigned_number(unsigned int n);
int		print_hexa(unsigned int n, char Uplo);
int		print_ptr(unsigned long n);
int		ptr_to_hexa(unsigned long ptr);
void	ft_putchar(char c);
char	*ft_strchr(const char *s, int c);

#endif