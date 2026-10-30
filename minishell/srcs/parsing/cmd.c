/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:03 by damalca           #+#    #+#             */
/*   Updated: 2026/02/12 14:24:16 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmds;
	t_command	*new;
	t_token		*cmd_end;

	if (!tokens)
		return (NULL);
	cmds = NULL;
	while (tokens)
	{
		cmd_end = tokens;
		while (cmd_end && cmd_end->type != PIPE)
			cmd_end = cmd_end->next;
		new = make_command(tokens, cmd_end);
		if (!new)
			return (NULL);
		add_cmd(&cmds, new);
		if (cmd_end && cmd_end->type == PIPE)
			tokens = cmd_end->next;
		else
			tokens = NULL;
	}
	return (cmds);
}

t_command	*make_command(t_token *tokens, t_token *cmd_end)
{
	t_command	*new;
	t_token		*tmp;

	new = create_command();
	if (!new)
		return (NULL);
	new->args = add_args(tokens, cmd_end);
	if (!new->args)
		return (free(new), NULL);
	tmp = tokens;
	handle_cmd_tokens(new, &tmp);
	return (new);
}

void	add_cmd(t_command **cmds, t_command *new)
{
	t_command	*last;

	if (!*cmds)
		*cmds = new;
	else
	{
		last = *cmds;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}
