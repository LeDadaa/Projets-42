/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 08:26:36 by damalca           #+#    #+#             */
/*   Updated: 2025/04/25 09:52:03 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

void	ft_write(int fd)
{
	char	buff[1024];
	ssize_t	bytes;

	bytes = read(fd, buff, 1024);
	while (bytes > 0)
	{
		write(1, buff, bytes);
		bytes = read(fd, buff, 1024);
	}
	if (bytes < 0)
		write(2, "Read error.\n", 12);
}

int	main(int ac, char **av)
{
	int	fd;

	if (ac != 2)
	{
		if (ac < 2)
			write(2, "File name missing.\n", 19);
		else
			write(2, "Too many arguments.\n", 20);
		return (1);
	}
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
	{
		write(2, "Cannot read file.\n", 18);
		return (1);
	}
	ft_write(fd);
	if (close(fd) < 0)
		return (1);
	return (0);
}
