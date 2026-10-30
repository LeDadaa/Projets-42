/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:03 by damalca           #+#    #+#             */
/*   Updated: 2026/02/12 14:19:14 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1)
		s1 = ft_strdup("");
	if (!s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

void	append_char(char **result, char c)
{
	char	str[2];
	char	*tmp;

	str[0] = c;
	str[1] = '\0';
	tmp = ft_strdup(str);
	*result = strjoin_free(*result, tmp);
	free(tmp);
}

int	is_var_start(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

char	*get_env_content(char *var_name, char **env, int status)
{
	char	*value;

	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(status));
	value = get_env_value(var_name, env);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*get_env_value(const char *name, char **env)
{
	int	i;
	int	name_len;

	if (!name || !env)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0
			&& env[i][name_len] == '=')
			return (env[i] + name_len + 1);
		i++;
	}
	return (NULL);
}
