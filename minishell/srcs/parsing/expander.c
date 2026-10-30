/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:03 by damalca           #+#    #+#             */
/*   Updated: 2026/02/04 15:18:46 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	expand_init(t_expand *ctx, char **env, int status)
{
	ctx->env = env;
	ctx->exit_status = status;
	ctx->result = ft_strdup("");
	ctx->sq = 0;
	ctx->dq = 0;
	ctx->i = 0;
}

static void	handle_quotes_state(char c, t_expand *ctx)
{
	if (c == '\'' && !ctx->dq)
	{
		ctx->sq = !ctx->sq;
		append_char(&ctx->result, c);
	}
	else if (c == '"' && !ctx->sq)
	{
		ctx->dq = !ctx->dq;
		append_char(&ctx->result, c);
	}
	else
		append_char(&ctx->result, c);
}

char	*process_expansion(char *str, char **env, int status)
{
	t_expand	ctx;

	expand_init(&ctx, env, status);
	if (!str || !ctx.result)
		return (NULL);
	while (str[ctx.i])
	{
		if (str[ctx.i] == '$' && !ctx.sq)
		{
			handle_dollar(str, &ctx.i, &ctx);
			continue ;
		}
		handle_quotes_state(str[ctx.i], &ctx);
		ctx.i++;
	}
	return (ctx.result);
}

void	expand_args(t_command *cmds, char **env, int exit_status)
{
	int		i;
	char	*tmp_expand;
	char	*final_str;

	while (cmds)
	{
		i = -1;
		while (cmds->args && cmds->args[++i])
		{
			tmp_expand = process_expansion(cmds->args[i], env, exit_status);
			if (tmp_expand)
			{
				final_str = remove_quotes_final(tmp_expand);
				free(cmds->args[i]);
				free(tmp_expand);
				cmds->args[i] = final_str;
			}
		}
		cmds = cmds->next;
	}
}

void	expand_redirections(t_command *cmds, char **env, int exit_status)
{
	t_redir	*redir;
	char	*tmp_expand;
	char	*final_str;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type != HEREDOC && redir->file)
			{
				tmp_expand = process_expansion(redir->file, env, exit_status);
				if (tmp_expand)
				{
					final_str = remove_quotes_final(tmp_expand);
					free(redir->file);
					free(tmp_expand);
					redir->file = final_str;
				}
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}
