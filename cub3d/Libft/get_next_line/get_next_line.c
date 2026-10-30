/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 14:30:56 by mchiacha          #+#    #+#             */
/*   Updated: 2026/03/13 16:21:01 by mchiacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_free2(char **str)
{
	free(*str);
	*str = NULL;
	return (NULL);
}

char	*clean_storage(char *storage)
{
	char	*new_storage;
	char	*ptr;
	int		len;

	ptr = ft_strchr1(storage, '\n');
	if (!ptr)
	{
		new_storage = NULL;
		return (ft_free2(&storage));
	}
	else
		len = (ptr - storage) + 1;
	if (!storage[len])
		return (ft_free2(&storage));
	new_storage = ft_substr1(storage, len, ft_strlen1(storage) - len);
	ft_free2(&storage);
	if (!new_storage)
		return (NULL);
	return (new_storage);
}

char	*new_line(char *storage)
{
	char	*line;
	char	*ptr;
	int		len;

	ptr = ft_strchr1(storage, '\n');
	len = (ptr - storage) + 1;
	line = ft_substr1(storage, 0, len);
	if (!line)
		return (NULL);
	return (line);
}

char	*readbuf(int fd, char *storage)
{
	int		rid;
	char	*buffer;

	rid = 1;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (ft_free2(&storage));
	buffer[0] = '\0';
	while (rid > 0 && !ft_strchr1(buffer, '\n'))
	{
		rid = read(fd, buffer, BUFFER_SIZE);
		if (rid > 0)
		{
			buffer[rid] = '\0';
			storage = ft_strjoin1(storage, buffer);
		}
	}
	free(buffer);
	if (rid == -1)
		return (ft_free2(&storage));
	return (storage);
}

char	*get_next_line(int fd)
{
	static char	*storage = {0};
	char		*line;

	if (fd < 0)
		return (free(storage), NULL);
	if ((storage && !ft_strchr1(storage, '\n')) || !storage)
		storage = readbuf(fd, storage);
	if (!storage)
		return (NULL);
	line = new_line(storage);
	if (!line)
		return (ft_free2(&storage));
	storage = clean_storage(storage);
	return (line);
}
