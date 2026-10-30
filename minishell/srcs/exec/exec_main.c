/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 13:16:12 by damalca           #+#    #+#             */
/*   Updated: 2026/02/26 01:30:14 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	cleanup_on_error(int **pipes, pid_t *pids, int allocated_pipes)
{
	int	i;

	if (pipes)
	{
		i = 0;
		while (i < allocated_pipes)
		{
			if (pipes[i])
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			i++;
		}
		free(pipes);
	}
	if (pids)
		free(pids);
}

static int	**allocate_pipes(int cmd_count, pid_t **pids, int *exit_status)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pipes || !*pids)
	{
		cleanup_on_error(pipes, *pids, 0);
		*exit_status = 1;
		return (NULL);
	}
	i = -1;
	while (++i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			cleanup_on_error(pipes, *pids, i);
			*exit_status = 1;
			return (NULL);
		}
	}
	return (pipes);
}

static void	exec_multiple_params(t_params *params)
{
	int			**pipes;
	pid_t		*pids;
	int			cmd_count;

	cmd_count = count_commands(params->cmds);
	pipes = allocate_pipes(cmd_count, &pids, params->exit_status);
	if (!pipes)
		return ;
	exec_pipeline(params, pipes, pids, cmd_count);
	cleanup_pipes(pipes, cmd_count - 1);
	wait_all_processes(pids, cmd_count, params->exit_status);
	free(pids);
}

static void	exec_single_command(t_params *params)
{
	int	builtin_type;

	builtin_type = is_builtin(params->cmds->args[0]);
	if (builtin_type == 1)
		exec_builtin(params->cmds, params->env, params->exit_status);
	else if (builtin_type == 2)
		exec_simple_builtin_parent(params->cmds, *params->env,
			params->exit_status);
	else if (is_variable_assignment(params->cmds->args[0]))
		*params->exit_status = 0;
	else
		exec_command(params);
}

void	exec_commands(t_params *params)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!params || !params->cmds)
		return ;
	if (process_heredocs(params->cmds, *params->env) == -1)
		*params->exit_status = 130;
	else if (!params->cmds->args || !params->cmds->args[0])
	{
		if (save_fds(&saved_stdin, &saved_stdout))
		{
			*params->exit_status = !handle_redir(params->cmds);
			restore_fds(saved_stdin, saved_stdout);
		}
	}
	else if (params->cmds->next == NULL)
		exec_single_command(params);
	else
		exec_multiple_params(params);
}
