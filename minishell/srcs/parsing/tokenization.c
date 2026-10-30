/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:03 by damalca           #+#    #+#             */
/*   Updated: 2026/02/18 23:12:01 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	length_word(char *cmd)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' && !dq)
			sq = !sq;
		else if (cmd[i] == '"' && !sq)
			dq = !dq;
		if (!sq && !dq && (is_space(cmd[i]) || handle_special(cmd + i)))
			break ;
		i++;
	}
	return (i);
}

static void	add_word(t_token **tokens, char **cmd)
{
	char	*str;
	int		length;

	length = length_word(*cmd);
	if (length == 0)
		return ;
	str = ft_substr(*cmd, 0, length);
	if (!str)
		return ;
	add_token(tokens, new_token(str, ARG));
	free(str);
	*cmd += length;
}

static void	add_special_token(t_token **tokens, char **cmd)
{
	int		spec;
	int		len;
	char	*special;

	spec = handle_special(*cmd);
	if (!spec)
		return ;
	len = 1;
	if (spec == HEREDOC || spec == APPEND)
		len = 2;
	special = ft_substr(*cmd, 0, len);
	if (special)
	{
		add_token(tokens, new_token(special, spec));
		free(special);
	}
	*cmd += len;
}

t_token	*tokenize(char *line)
{
	t_token	*tokens;

	tokens = NULL;
	while (*line)
	{
		while (*line && is_space(*line))
			line++;
		if (!*line)
			break ;
		if (handle_special(line))
			add_special_token(&tokens, &line);
		else
			add_word(&tokens, &line);
	}
	return (tokens);
}
