/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:21:32 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 13:08:05 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				result[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static char	*process_dollar_expansion(char *line, int *i, char **env)
{
	char	*expanded;

	if (line[*i + 1] == '?')
	{
		expanded = expand_exit_status(line, i, g_last_status);
		free(line);
		return (expanded);
	}
	else if (line[*i + 1] == '$')
	{
		expanded = expand_pid(line, i);
		free(line);
		return (expanded);
	}
	else if (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_'
		|| line[*i + 1] == '{')
	{
		expanded = expand_variable(line, i, env);
		free(line);
		return (expanded);
	}
	return (line);
}

char	*expand_variables(char *line, char **env, int exit_status)
{
	int		i;

	(void)exit_status;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			line = process_dollar_expansion(line, &i, env);
		i++;
	}
	return (line);
}

char	*get_clean_delimiter(t_command *cmd, int *should_expand)
{
	char	*clean_delim;

	*should_expand = !has_quotes(cmd->heredoc_delim);
	if (*should_expand)
		return (cmd->heredoc_delim);
	clean_delim = remove_quotes(cmd->heredoc_delim);
	if (!clean_delim)
		return (cmd->heredoc_delim);
	return (clean_delim);
}
