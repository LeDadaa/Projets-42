/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:00:00 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 13:13:35 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_tilde(char *path, char **env)
{
	char	*home;
	char	*expanded;
	size_t	len;

	if (!path || path[0] != '~')
		return (path);
	if (path[1] != '\0' && path[1] != '/')
		return (path);
	home = get_env_value_cd(env, "HOME");
	if (!home)
		return (path);
	if (path[1] == '\0')
		return (home);
	len = ft_strlen(home) + ft_strlen(path + 1) + 1;
	expanded = malloc(len);
	if (!expanded)
		return (path);
	ft_strlcpy(expanded, home, len);
	ft_strlcat(expanded, path + 1, len);
	return (expanded);
}

char	*handle_home_dir(char ***env, char **oldpwd)
{
	char	*target;

	target = get_env_value_cd(*env, "HOME");
	if (!target)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		free(*oldpwd);
		*oldpwd = NULL;
	}
	return (target);
}

char	*handle_oldpwd_dir(char ***env, char **oldpwd)
{
	char	*target;

	target = get_env_value_cd(*env, "OLDPWD");
	if (!target)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		free(*oldpwd);
		*oldpwd = NULL;
		return (NULL);
	}
	ft_putstr_fd(target, 1);
	ft_putstr_fd("\n", 1);
	return (NULL);
}

int	do_chdir(char *target, char *oldpwd, char **args, int need_free)
{
	if (chdir(target) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(oldpwd);
		if (need_free)
			free(target);
		return (1);
	}
	return (0);
}

int	update_pwd_vars(char *oldpwd, char *newpwd, char ***env)
{
	update_env_var(env, "OLDPWD", oldpwd);
	update_env_var(env, "PWD", newpwd);
	free(newpwd);
	free(oldpwd);
	return (0);
}
