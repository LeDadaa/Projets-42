/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 15:06:32 by damalca          #+#    #+#             */
/*   Updated: 2025/11/25 00:30:27 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern char	**environ;

static char	*get_target_dir(char **args, char ***env, char **oldpwd)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		return (handle_home_dir(env, oldpwd));
	if (ft_strcmp(args[1], "-") == 0)
		return (handle_oldpwd_dir(env, oldpwd));
	return (NULL);
}

static int	get_current_pwd(char **oldpwd, char **newpwd, int need_free,
	char *target)
{
	*newpwd = getcwd(NULL, 0);
	if (!*newpwd)
		*newpwd = ft_strdup("");
	if (!*newpwd)
	{
		free(*oldpwd);
		if (need_free)
			free(target);
		return (1);
	}
	return (0);
}

static int	perform_cd(char *target, char **args, char ***env, int need_free)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = ft_strdup("");
	if (!oldpwd)
		return (1);
	if (do_chdir(target, oldpwd, args, need_free))
		return (1);
	if (get_current_pwd(&oldpwd, &newpwd, need_free, target))
		return (1);
	if (need_free)
		free(target);
	return (update_pwd_vars(oldpwd, newpwd, env));
}

static char	*get_cd_target(char **args, char ***env, int *need_free)
{
	char	*target;
	char	*expanded;

	*need_free = 0;
	target = get_target_dir(args, env, NULL);
	if (!target && args[1])
	{
		expanded = expand_tilde(args[1], *env);
		*need_free = (expanded != args[1]);
		target = expanded;
	}
	return (target);
}

int	ft_cd(char **args, char ***env)
{
	char	*target;
	int		need_free;

	if (!args || !env || !*env)
		return (1);
	if (args[1] && args[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	target = get_cd_target(args, env, &need_free);
	return (perform_cd(target, args, env, need_free));
}
