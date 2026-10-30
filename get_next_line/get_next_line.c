/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:19:30 by damalca           #+#    #+#             */
/*   Updated: 2025/05/14 13:04:22 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_and_store(int fd, char *stash)
{
	char	*buffer;
	char	*temp;
	ssize_t	bytes;

	stash = init_stash(stash);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free(stash), NULL);
	if (!stash)
		return (NULL);
	bytes = 1;
	while (!ft_strchr(stash, '\n') && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (free(buffer), free(stash), NULL);
		buffer[bytes] = '\0';
		temp = ft_strjoin(stash, buffer);
		if (!temp)
			return (free(stash), free(buffer), NULL);
		free(stash);
		stash = temp;
	}
	free(buffer);
	return (stash);
}

static char	*get_line(char *stash)
{
	int		len;
	int		y;
	char	*line;

	if (!stash || stash[0] == '\0')
		return (NULL);
	len = line_len(stash);
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	y = 0;
	while (y < len)
	{
		line[y] = stash[y];
		y++;
	}
	line[y] = '\0';
	return (line);
}

static char	*new_stash(char *stash)
{
	size_t	s;
	size_t	ns;
	char	*new_stash;

	s = 0;
	while (stash[s] && stash[s] != '\n')
		s++;
	if (!stash[s])
		return (free(stash), NULL);
	s++;
	new_stash = malloc(ft_strlen(stash) - s + 1);
	if (!new_stash)
		return (NULL);
	ns = 0;
	while (stash[s])
		new_stash[ns++] = stash[s++];
	new_stash[ns] = '\0';
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	stash = read_and_store(fd, stash);
	if (!stash)
		return (NULL);
	line = get_line(stash);
	stash = new_stash(stash);
	return (line);
}
/*
#include <stdio.h>
int	main()
{
	int	fd;
	char *line;
	fd = open("test.txt", O_RDONLY);
	if (fd < 0)
	{
		write(2, "Error", 5);
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}*/