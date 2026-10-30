/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 11:09:04 by damalca           #+#    #+#             */
/*   Updated: 2026/02/19 00:45:42 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	exec_builtin_cmd(t_command *cmds, char ***env,
		int *exit_status, int *saved_fds)
{
	if (ft_strcmp(cmds->args[0], "cd") == 0)
		*exit_status = ft_cd(cmds->args, env);
	else if (ft_strcmp(cmds->args[0], "export") == 0)
		*exit_status = ft_export(cmds->args, env);
	else if (ft_strcmp(cmds->args[0], "unset") == 0)
		*exit_status = ft_unset(cmds->args, env);
	else if (ft_strcmp(cmds->args[0], "exit") == 0)
	{
		ft_exit(cmds->args, exit_status, saved_fds[0], saved_fds[1]);
	}
}

void	exec_builtin(t_command *cmds, char ***env, int *exit_status)
{
	int	saved_fds[2];

	if (!prepare_builtin(cmds, &saved_fds[0], &saved_fds[1]))
	{
		*exit_status = 1;
		return ;
	}
	exec_builtin_cmd(cmds, env, exit_status, saved_fds);
	restore_fds(saved_fds[0], saved_fds[1]);
}

static void	exec_simple_builtin_cmd(t_command *cmds,
		char **env, int *exit_status)
{
	if (ft_strcmp(cmds->args[0], "echo") == 0)
		*exit_status = ft_echo(cmds->args);
	else if (ft_strcmp(cmds->args[0], "pwd") == 0)
		*exit_status = ft_pwd();
	else if (ft_strcmp(cmds->args[0], "env") == 0)
		*exit_status = ft_env(env);
}

void	exec_simple_builtin_parent(t_command *cmds,
		char **env, int *exit_status)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!prepare_builtin(cmds, &saved_stdin, &saved_stdout))
	{
		*exit_status = 1;
		return ;
	}
	exec_simple_builtin_cmd(cmds, env, exit_status);
	restore_fds(saved_stdin, saved_stdout);
}
