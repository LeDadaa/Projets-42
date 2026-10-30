/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 12:43:26 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 13:08:38 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	exec_env_modifying_builtin(t_command *cmd, char **env, int *status)
{
	char	***env_ptr;

	env_ptr = &env;
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		*status = ft_cd(cmd->args, env_ptr);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		*status = ft_export(cmd->args, env_ptr);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		*status = ft_unset(cmd->args, env_ptr);
}

void	exec_builtin_type(t_command *cmd, char **env, int *status)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		*status = ft_echo(cmd->args);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		*status = ft_pwd();
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		*status = ft_env(env);
	else
		exec_env_modifying_builtin(cmd, env, status);
}
