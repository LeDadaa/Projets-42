/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:45:00 by damalca           #+#    #+#             */
/*   Updated: 2026/02/15 12:10:14 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern char	**environ;

int	is_valid_variable(char *str)
{
	int	i;

	if (!str || !str[0] || ft_isdigit(str[0]) || str[0] == '=')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	**alloc_new_env(char ***env, char *new_var, int count)
{
	char	**new_env;
	int		j;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return (NULL);
	}
	j = 0;
	while (j < count)
	{
		new_env[j] = (*env)[j];
		j++;
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	return (new_env);
}

static char	*create_export_var(char *name, char *value)
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

static int	update_existing_var(char ***env, char *name, char *new_var)
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

void	update_or_add_env(char ***env, char *name, char *value)
{
	char	*new_var;
	char	**new_env;
	int		count;

	if (!env || !name || !*name || !value)
		return ;
	new_var = create_export_var(name, value);
	if (!new_var)
		return ;
	if (update_existing_var(env, name, new_var))
		return ;
	count = 0;
	while ((*env)[count])
		count++;
	new_env = alloc_new_env(env, new_var, count);
	if (new_env)
	{
		free(*env);
		*env = new_env;
		environ = *env;
	}
}
