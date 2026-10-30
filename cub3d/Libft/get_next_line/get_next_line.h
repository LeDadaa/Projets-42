/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 14:31:17 by mchiacha          #+#    #+#             */
/*   Updated: 2025/11/15 17:21:12 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <limits.h>

size_t	ft_strlen1(char *s);
char	*ft_strjoin1(char *s1, char *s2);
char	*ft_strchr1(char *s, int c);
char	*ft_substr1(char *s, unsigned int start, size_t len);
char	*get_next_line(int fd);
char	*ft_free2(char **str);
#endif
