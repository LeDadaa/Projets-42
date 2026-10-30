/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damalca <damalca@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 11:40:08 by damalca           #+#    #+#             */
/*   Updated: 2026/02/15 13:54:47 by damalca          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setup_pipe_fds(int **pipes, int cmd_index, int cmd_count)
{
	if (cmd_index > 0)
		dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < cmd_count - 1)
		dup2(pipes[cmd_index][1], STDOUT_FILENO);
}

void	close_all_pipes(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	exec_builtin_in_child(t_command *cmd, char **env)
{
	int	status;

	status = 0;
	exec_builtin_type(cmd, env, &status);
	exit(status);
}

void	handle_cmd_path_error(char *cmd_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_name, 2);
	if (ft_strchr(cmd_name, '/'))
	{
		if (access(cmd_name, F_OK) != 0)
		{
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(127);
		}
		if (access(cmd_name, X_OK) != 0)
		{
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
	}
	ft_putstr_fd(": command not found\n", 2);
	exit(127);
}
