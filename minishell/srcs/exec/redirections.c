/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 11:18:40 by damalca           #+#    #+#             */
/*   Updated: 2026/02/18 21:47:52 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_input_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->file);
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

static int	handle_output_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->file);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

static int	process_heredoc_dup(t_command *cmds)
{
	if (cmds->heredoc_fd != -1)
	{
		if (dup2(cmds->heredoc_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 heredoc");
			close(cmds->heredoc_fd);
			return (0);
		}
		close(cmds->heredoc_fd);
		cmds->heredoc_fd = -1;
	}
	return (1);
}

int	handle_redir(t_command *cmds)
{
	t_redir	*redir;

	if (!cmds)
		return (1);
	if (!process_heredoc_dup(cmds))
		return (0);
	redir = cmds->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN && !handle_input_redir(redir))
			return (0);
		else if ((redir->type == REDIR_OUT || redir->type == APPEND)
			&& !handle_output_redir(redir))
			return (0);
		redir = redir->next;
	}
	return (1);
}
