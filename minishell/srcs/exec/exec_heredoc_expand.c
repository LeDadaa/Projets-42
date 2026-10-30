/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 21:02:32 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 13:07:46 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_pid(char *line, int *i)
{
	char	*before;
	char	*pid_str;
	char	*after;
	char	*result;

	before = ft_substr(line, 0, *i);
	pid_str = ft_itoa(getpid());
	after = ft_strdup(line + *i + 2);
	result = ft_strjoin(before, pid_str);
	free(before);
	free(pid_str);
	before = result;
	result = ft_strjoin(before, after);
	free(before);
	free(after);
	return (result);
}

char	*expand_exit_status(char *line, int *i, int exit_status)
{
	char	*before;
	char	*status_str;
	char	*after;
	char	*result;

	before = ft_substr(line, 0, *i);
	status_str = ft_itoa(exit_status);
	after = ft_strdup(line + *i + 2);
	result = ft_strjoin(before, status_str);
	free(before);
	free(status_str);
	before = result;
	result = ft_strjoin(before, after);
	free(before);
	free(after);
	return (result);
}

static char	*extract_var_name(char *line, int *i, int *end)
{
	char	*var_name;
	int		j;

	j = *i + 1;
	if (line[j] == '{')
	{
		j++;
		while (line[j] && line[j] != '}')
			j++;
		var_name = ft_substr(line, *i + 2, j - *i - 2);
		j++;
	}
	else
	{
		while (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
			j++;
		var_name = ft_substr(line, *i + 1, j - *i - 1);
	}
	*end = j;
	return (var_name);
}

static char	*build_expanded_string(char *line, int start, int end, char *value)
{
	char	*before;
	char	*after;
	char	*result;

	before = ft_substr(line, 0, start);
	after = ft_strdup(line + end);
	result = ft_strjoin(before, value);
	free(before);
	before = result;
	result = ft_strjoin(before, after);
	free(before);
	free(after);
	return (result);
}

char	*expand_variable(char *line, int *i, char **env)
{
	char	*var_name;
	char	*var_value;
	char	*result;
	int		end;

	var_name = extract_var_name(line, i, &end);
	var_value = get_env_value_cd(env, var_name);
	free(var_name);
	if (!var_value)
		var_value = "";
	result = build_expanded_string(line, *i, end, var_value);
	*i += ft_strlen(var_value) - 1;
	return (result);
}
