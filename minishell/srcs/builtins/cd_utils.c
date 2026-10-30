/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:40:00 by damalca           #+#    #+#             */
/*   Updated: 2025/11/25 00:30:23 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern char	**environ;

static char	**create_new_env(char ***env, char *new_var, int count)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new_env[i] = (*env)[i];
		i++;
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free(*env);
	return (new_env);
}

static char	*create_var_string(char *name, char *value)
{
	size_t	len;
	char	*new_var;

	len = ft_strlen(name) + ft_strlen(value) + 2;
	new_var = malloc(len);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, name, len);
	ft_strlcat(new_var, "=", len);
	ft_strlcat(new_var, value, len);
	return (new_var);
}

static int	try_update_existing(char ***env, char *name, char *new_var)
{
	int	i;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], name, ft_strlen(name)) == 0
			&& (*env)[i][ft_strlen(name)] == '=')
		{
			free((*env)[i]);
			(*env)[i] = new_var;
			environ = *env;
			return (1);
		}
		i++;
	}
	return (0);
}

void	update_env_var(char ***env, char *name, char *value)
{
	char	*new_var;
	char	**new_env;
	int		count;

	if (!env || !*env || !name || !*name || !value)
		return ;
	new_var = create_var_string(name, value);
	if (!new_var)
		return ;
	if (try_update_existing(env, name, new_var))
		return ;
	count = 0;
	while ((*env)[count])
		count++;
	new_env = create_new_env(env, new_var, count);
	if (new_env)
	{
		*env = new_env;
		environ = *env;
	}
}

char	*get_env_value_cd(char **env, const char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}
