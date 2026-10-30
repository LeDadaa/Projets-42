/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:33:29 by damalca           #+#    #+#             */
/*   Updated: 2026/02/18 23:24:11 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_last_status = 0;

static int	init_params_and_exec(char ***shell_env, t_token *tokens,
		t_command *cmds, int *exit_status)
{
	t_params	params;

	params.env = shell_env;
	params.cmds = cmds;
	params.exit_status = exit_status;
	params.tokens = tokens;
	params.shell_env = *shell_env;
	exec_commands(&params);
	free_tokens(tokens);
	free_cmds(cmds);
	return (1);
}

static int	process_line(char *line, char ***shell_env, int *exit_status)
{
	t_token		*tokens;
	t_command	*cmds;

	if (!validate_quotes(line))
		return (ft_putstr_fd("minishell: unclosed quote\n", 2), 0);
	tokens = tokenize(line);
	if (!tokens)
		return (0);
	cmds = parse_tokens(tokens);
	if (!cmds)
		return (free_tokens(tokens), 0);
	expand_args(cmds, *shell_env, *exit_status);
	expand_redirections(cmds, *shell_env, *exit_status);
	init_params_and_exec(shell_env, tokens, cmds, exit_status);
	return (1);
}

static int	parse_input(char ***shell_env, int *exit_status)
{
	char		*line;

	line = readline("minishell$ ");
	if (!line)
		return (printf("exit\n"), 0);
	if (*line)
		add_history(line);
	if (!process_line(line, shell_env, exit_status))
	{
		free(line);
		return (1);
	}
	free(line);
	return (1);
}

static void	minishell_loop(char ***shell_env, int *exit_status)
{
	while (1)
	{
		if (!parse_input(shell_env, exit_status))
			break ;
		if (g_last_status < 0)
		{
			if (g_last_status == -1)
				*exit_status = 0;
			else if (g_last_status == -2)
				*exit_status = 2;
			else
				*exit_status = -(g_last_status + 1000);
			break ;
		}
		g_last_status = *exit_status;
	}
}

int	main(int argc, char **argv, char **env)
{
	char	**shell_env;
	int		exit_status;

	(void)argc;
	(void)argv;
	exit_status = 0;
	shell_env = duplicate_env(env);
	if (!shell_env)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	init_signals_main();
	minishell_loop(&shell_env, &exit_status);
	rl_clear_history();
	free_env(shell_env);
	return (exit_status);
}
