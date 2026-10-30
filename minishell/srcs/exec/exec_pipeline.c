/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 13:52:57 by damalca           #+#    #+#             */
/*   Updated: 2026/02/18 21:42:48 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	wait_all_processes(pid_t *pids, int cmd_count, int *exit_status)
{
	int	i;
	int	status;

	init_signals_exec();
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				*exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	init_signals_main();
}

static void	exec_external_in_pipe(t_command *cmd, char **env)
{
	char	*cmd_path;

	cmd_path = find_command_path(cmd->args[0], env);
	if (!cmd_path)
		handle_cmd_path_error(cmd->args[0]);
	if (execve(cmd_path, cmd->args, env) == -1)
	{
		perror("minishell");
		free(cmd_path);
		exit(126);
	}
}

static void	exec_pipe_cmd(t_command *cmd, char **env, int **pipes, int *tab)
{
	int	builtin_type;

	setup_pipe_fds(pipes, tab[0], tab[1]);
	close_all_pipes(pipes, tab[1] - 1);
	if (!handle_redir(cmd))
		exit(1);
	builtin_type = is_builtin(cmd->args[0]);
	if (builtin_type == 1 || builtin_type == 2)
	{
		exec_builtin_in_child(cmd, env);
		exit(0);
	}
	exec_external_in_pipe(cmd, env);
}

void	exec_pipeline(t_params *params, int **pipes, pid_t *pids, int cmd_count)
{
	t_command	*current;
	int			i;
	int			tab[2];

	current = params->cmds;
	i = 0;
	while (current && i < cmd_count)
	{
		tab[0] = i;
		tab[1] = cmd_count;
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			return ;
		}
		if (pids[i] == 0)
		{
			init_signals_child();
			exec_pipe_cmd(current, *(params->env), pipes, tab);
			exit(1);
		}
		current = current->next;
		i++;
	}
}
