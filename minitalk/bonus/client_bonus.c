/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:15:03 by damalca           #+#    #+#             */
/*   Updated: 2025/08/25 15:20:03 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minitalk.h"

volatile sig_atomic_t	g_ack = 0;

static void	handle_ack(int signal)
{
	(void)signal;
	g_ack = 1;
}

void	send_signal(int pid, unsigned char character)
{
	int	i;

	i = 8;
	while (i--)
	{
		if ((character >> i) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		while (!g_ack)
			pause();
		g_ack = 0;
	}
	ft_printf("Byte sent\n");
	if (character == '\0')
		ft_printf("Transmission over\n");
}

int	main(int argc, char *argv[])
{
	pid_t		server_pid;
	const char	*message;
	int			i;

	if (argc != 3)
	{
		ft_printf("Usage: %s <pid> <message>\n", argv[0]);
		exit(0);
	}
	server_pid = ft_atoi(argv[1]);
	if (server_pid <= 0)
		return (0);
	signal(SIGUSR1, handle_ack);
	signal(SIGUSR2, handle_ack);
	message = argv[2];
	i = 0;
	while (message[i])
		send_signal(server_pid, message[i++]);
	send_signal(server_pid, '\0');
	return (0);
}
