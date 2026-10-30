/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 14:17:38 by damalca           #+#    #+#             */
/*   Updated: 2026/02/19 00:30:02 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_heredoc_line(char *line, int fd, char **env, int expand)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_variables(line, env, g_last_status);
		if (expanded)
		{
			write(fd, expanded, ft_strlen(expanded));
			write(fd, "\n", 1);
		}
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

static int	write_heredoc_to_pipe(int pipe_fd[2], char *delimiter,
		char **env, int expand_var)
{
	char	*line;
	int		interrupted;

	interrupted = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_last_status == 130)
				interrupted = 1;
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(line, pipe_fd[1], env, expand_var);
		free(line);
	}
	return (interrupted);
}

int	handle_heredoc(char *delimiter, char **env, int expand_var)
{
	int	pipe_fd[2];
	int	interrupted;
	int	stdin_backup;

	if (!delimiter)
		return (-1);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	stdin_backup = dup(STDIN_FILENO);
	g_last_status = 0;
	init_signals_heredoc();
	interrupted = write_heredoc_to_pipe(pipe_fd, delimiter, env, expand_var);
	init_signals_main();
	if (interrupted)
	{
		dup2(stdin_backup, STDIN_FILENO);
		close(stdin_backup);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		g_last_status = 130;
		return (-1);
	}
	close(stdin_backup);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
