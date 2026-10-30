/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 13:58:54 by damalca           #+#    #+#             */
/*   Updated: 2026/02/21 13:07:33 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_cmd_error(char *cmd, int *exit_status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	if (ft_strchr(cmd, '/') != NULL)
	{
		if (access(cmd, F_OK) != 0)
		{
			ft_putstr_fd(": No such file or directory\n", 2);
			*exit_status = 127;
		}
		else
		{
			ft_putstr_fd(": Permission denied\n", 2);
			*exit_status = 126;
		}
	}
	else
	{
		ft_putstr_fd(": command not found\n", 2);
		*exit_status = 127;
	}
}

static void	exec_in_child(char *cmd_path, t_command *cmd, char **env)
{
	if (!handle_redir(cmd))
		exit(1);
	if (execve(cmd_path, cmd->args, env) == -1)
	{
		perror("minishell");
		free(cmd_path);
		exit(126);
	}
}

static void	wait_for_child(pid_t pid, int *exit_status)
{
	int	status;

	init_signals_exec();
	waitpid(pid, &status, 0);
	init_signals_main();
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*exit_status = 128 + WTERMSIG(status);
}

static int	fork_and_exec(char *cmd_path, t_params *params)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		*params->exit_status = 1;
		return (0);
	}
	if (pid == 0)
	{
		init_signals_child();
		exec_in_child(cmd_path, params->cmds, *(params->env));
		exit(1);
	}
	free(cmd_path);
	wait_for_child(pid, params->exit_status);
	return (1);
}

void	exec_command(t_params *params)
{
	char	*cmd_path;

	cmd_path = find_command_path(params->cmds->args[0], *(params->env));
	if (!cmd_path)
	{
		handle_cmd_error(params->cmds->args[0], params->exit_status);
		return ;
	}
	fork_and_exec(cmd_path, params);
}
