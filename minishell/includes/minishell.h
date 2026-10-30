/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:49:44 by damalca           #+#    #+#             */
/*   Updated: 2026/02/26 01:37:58 by tmottini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include "../Libft/libft.h"

# include <string.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

extern int	g_last_status;

typedef enum TokenType
{
	CMD,
	ARG,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
}		t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}			t_token;

typedef struct s_redir
{
	int					type;
	char				*file;
	struct s_redir		*next;
	int					heredoc_quotes;
}			t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*redirs;
	char				*heredoc_delim;
	int					heredoc_fd;
	struct s_command	*next;
}			t_command;

typedef struct s_params
{
	char		***env;
	t_command	*cmds;
	int			*exit_status;
	t_token		*tokens;
	char		**shell_env;
}			t_params;

typedef struct s_expand
{
	char	**env;
	int		exit_status;
	char	*result;
	int		sq;
	int		dq;
	int		i;
}			t_expand;

//BUILTINS
int			ft_cd(char **args, char ***env);
void		update_env_var(char ***env, char *name, char *value);
char		*get_env_value_cd(char **env, const char *name);
char		*expand_tilde(char *path, char **env);
char		*handle_home_dir(char ***env, char **oldpwd);
char		*handle_oldpwd_dir(char ***env, char **oldpwd);
int			do_chdir(char *target, char *oldpwd, char **args, int need_free);
int			update_pwd_vars(char *oldpwd, char *newpwd, char ***env);
int			ft_echo(char **args);
int			ft_pwd(void);
int			ft_env(char **env);
char		**duplicate_env(char **env);
void		free_env(char **env);
int			ft_export(char **args, char ***env);
int			is_valid_variable(char *str);
void		update_or_add_env(char ***env, char *name, char *value);
void		print_sorted_env(char **env);
int			ft_unset(char **args, char ***env);
void		ft_exit(char **args, int *exit_status, int fd1, int fd2);

//EXEC
void		exec_commands(t_params *params);
int			is_builtin(char *cmd);
int			is_variable_assignment(char *arg);
char		*find_command_path(char *cmd, char **env);
void		exec_command(t_params *params);
void		exec_builtin(t_command *cmds, char ***env, int *exit_status);
void		exec_simple_builtin_parent(t_command *cmds, char **env,
				int *exit_status);
void		exec_builtin_in_child(t_command *cmd, char **env);
int			handle_redir(t_command *cmds);
void		exec_pipeline(t_params *params, int **pipes, pid_t *pids,
				int cmd_count);
void		cleanup_pipes(int **pipes, int pipe_count);
void		wait_all_processes(pid_t *pids, int cmd_count, int *exit_status);
int			handle_heredoc(char *delimiter, char **env, int expand_var);
int			process_heredocs(t_command *cmds, char **env);
int			count_commands(t_command *cmds);
char		*remove_quotes(char *str);
char		*expand_variables(char *line, char **env, int exit_status);
char		*expand_variable(char *line, int *i, char **env);
char		*expand_exit_status(char *line, int *i, int exit_status);
char		*expand_pid(char *line, int *i);
int			prepare_builtin(t_command *cmds,
				int *saved_stdin, int *saved_stdout);
int			save_fds(int *saved_stdin, int *saved_stdout);
void		restore_fds(int saved_stdin, int saved_stdout);
void		handle_cmd_path_error(char *cmd_name);
void		exec_builtin_in_child(t_command *cmd, char **env);
void		close_all_pipes(int **pipes, int pipe_count);
void		setup_pipe_fds(int **pipes, int cmd_index, int cmd_count);
void		exec_builtin_type(t_command *cmd, char **env, int *status);
char		*get_clean_delimiter(t_command *cmd, int *should_expand);
char		*expand_variables(char *line, char **env, int exit_status);
char		*remove_quotes(char *str);

// FREE
void		free_cmds(t_command *cmds);
void		free_tokens(t_token *tokens);

// OKENIZATION
t_token		*tokenize(char *line);
t_token		*new_token(char *value, int type);
void		add_token(t_token **head, t_token *tok);
int			is_space(char c);
int			handle_special(char *str);
int			validate_quotes(char *line);
int			is_in_single_quotes(char *str, int pos);
int			is_in_quotes(char *str, int pos);

// CMD
t_command	*parse_tokens(t_token *tokens);
t_command	*make_command(t_token *tokens, t_token *cmd_end);
t_command	*create_command(void);
void		add_cmd(t_command **cmds, t_command *new);
void		handle_cmd_tokens(t_command *cmd, t_token **tokens);
char		**add_args(t_token *start, t_token *end);

// EXPANSION
void		expand_args(t_command *cmds, char **env, int exit_status);
void		expand_redirections(t_command *cmds, char **env, int exit_status);
char		*process_expansion(char *str, char **env, int status);
char		*strjoin_free(char *s1, char *s2);
void		append_char(char **result, char c);
int			is_var_start(char c);
char		*get_env_content(char *var_name, char **env, int status);
void		handle_dollar(char *str, int *i, t_expand *ctx);
char		*get_env_value(const char *name, char **env);
char		*remove_quotes_final(char *str);

//SIGNALS
void		sigint_handler(int sig);
void		heredoc_sigint_handler(int sig);
void		init_signals_main(void);
void		init_signals_child(void);
void		init_signals_heredoc(void);
void		init_signals_exec(void);
void		sigint_child_wait_handler(int sig);

#endif