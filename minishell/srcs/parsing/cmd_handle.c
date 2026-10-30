/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:03 by damalca           #+#    #+#             */
/*   Updated: 2026/02/15 15:07:55 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_fd = -1;
	cmd->next = NULL;
	return (cmd);
}

static void	add_redir(t_command *cmd, int type, char *file)
{
	t_redir	*new;
	t_redir	*curr;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->type = type;
	new->file = ft_strdup(file);
	new->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		curr = cmd->redirs;
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
}

void	handle_cmd_tokens(t_command *cmd, t_token **tokens)
{
	while (*tokens && (*tokens)->type != PIPE)
	{
		if ((*tokens)->type != ARG)
		{
			if ((*tokens)->next)
			{
				add_redir(cmd, (*tokens)->type, (*tokens)->next->value);
				*tokens = (*tokens)->next;
			}
		}
		if (*tokens)
			*tokens = (*tokens)->next;
	}
}
