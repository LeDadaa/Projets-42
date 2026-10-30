/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:03 by damalca           #+#    #+#             */
/*   Updated: 2026/02/26 01:32:49 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_heredoc_delimiter(t_redir *redir)
{
	char	*clean;

	redir->heredoc_quotes = 0;
	if (ft_strchr(redir->file, '\'') || ft_strchr(redir->file, '"'))
	{
		redir->heredoc_quotes = 1;
		clean = remove_quotes_final(redir->file);
		if (clean)
		{
			free(redir->file);
			redir->file = clean;
		}
	}
}

static int	handle_single_redir(t_command *cmd, t_redir *redir, char **env)
{
	int	result;

	if (redir->type == HEREDOC)
	{
		process_heredoc_delimiter(redir);
		if (cmd->heredoc_fd > 0)
			close(cmd->heredoc_fd);
		result = handle_heredoc(redir->file, env, !redir->heredoc_quotes);
		cmd->heredoc_fd = result;
		if (result == -1 && g_last_status == 130)
			return (-1);
	}
	return (0);
}

static int	process_cmd_heredocs(t_command *cmd, char **env)
{
	t_redir	*curr_redir;

	curr_redir = cmd->redirs;
	while (curr_redir)
	{
		if (handle_single_redir(cmd, curr_redir, env) == -1)
			return (-1);
		curr_redir = curr_redir->next;
	}
	return (0);
}

int	process_heredocs(t_command *cmds, char **env)
{
	t_command	*curr_cmd;

	curr_cmd = cmds;
	while (curr_cmd)
	{
		if (process_cmd_heredocs(curr_cmd, env) == -1)
			return (-1);
		curr_cmd = curr_cmd->next;
	}
	return (0);
}
