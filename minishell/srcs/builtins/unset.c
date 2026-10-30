/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:25:18 by damalca           #+#    #+#             */
/*   Updated: 2025/11/25 00:31:41 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern char	**environ;

static void	remove_env_var(char **env, int index)
{
	int	i;

	free(env[index]);
	i = index;
	while (env[i])
	{
		env[i] = env[i + 1];
		i++;
	}
}

static char	*get_var_name(char *env_entry)
{
	char	*eq_pos;

	eq_pos = ft_strchr(env_entry, '=');
	if (!eq_pos)
		return (NULL);
	return (ft_substr(env_entry, 0, eq_pos - env_entry));
}

static int	check_and_unset(char **env, char *arg)
{
	char	*str;
	int		j;

	j = 0;
	while (env[j])
	{
		str = get_var_name(env[j]);
		if (!str)
		{
			j++;
			continue ;
		}
		if (ft_strcmp(str, arg) == 0)
		{
			free(str);
			remove_env_var(env, j);
			return (1);
		}
		free(str);
		j++;
	}
	return (0);
}

int	ft_unset(char **args, char ***env)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (check_and_unset(*env, args[i]))
			environ = *env;
		i++;
	}
	return (0);
}
