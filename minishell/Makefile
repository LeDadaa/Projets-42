# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmottini <tmottini@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/16 14:44:18 by damalca           #+#    #+#              #
#    Updated: 2026/02/26 01:36:15 by tmottini         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g -Iincludes -ILibft
LDFLAGS     = -lreadline

LIBFT_DIR   = Libft
LIBFT       = $(LIBFT_DIR)/libft.a

SRCS_DIR    = srcs
BUILTINS_DIR= $(SRCS_DIR)/builtins
EXEC_DIR    = $(SRCS_DIR)/exec
FREE_DIR    = $(SRCS_DIR)/free
PARSING_DIR = $(SRCS_DIR)/parsing
SIGNALS_DIR = $(SRCS_DIR)/signals

SRCS        = $(SRCS_DIR)/minishell.c \
              $(BUILTINS_DIR)/cd.c \
              $(BUILTINS_DIR)/cd_helpers.c \
              $(BUILTINS_DIR)/cd_utils.c \
              $(BUILTINS_DIR)/echo.c \
              $(BUILTINS_DIR)/env.c \
              $(BUILTINS_DIR)/env_utils.c \
              $(BUILTINS_DIR)/exit.c \
              $(BUILTINS_DIR)/export.c \
              $(BUILTINS_DIR)/export_sort.c \
              $(BUILTINS_DIR)/export_utils.c \
              $(BUILTINS_DIR)/pwd.c \
              $(BUILTINS_DIR)/unset.c \
              $(EXEC_DIR)/exec_builtin_fds.c \
              $(EXEC_DIR)/exec_command.c \
              $(EXEC_DIR)/exec_builtin.c \
              $(EXEC_DIR)/exec_heredoc_expand.c \
              $(EXEC_DIR)/exec_heredoc_utils.c \
              $(EXEC_DIR)/exec_heredoc.c \
              $(EXEC_DIR)/exec_main.c \
              $(EXEC_DIR)/exec_main_utils.c \
              $(EXEC_DIR)/exec_path.c \
              $(EXEC_DIR)/exec_pipe_builtins.c \
              $(EXEC_DIR)/exec_pipeline_child.c \
              $(EXEC_DIR)/exec_pipeline.c \
              $(EXEC_DIR)/exec_utils.c \
              $(EXEC_DIR)/redirections.c \
              $(PARSING_DIR)/cmd.c \
              $(PARSING_DIR)/cmd_args.c \
              $(PARSING_DIR)/cmd_handle.c \
              $(PARSING_DIR)/expand_utils.c \
              $(PARSING_DIR)/expand_var.c \
              $(PARSING_DIR)/expander.c \
              $(PARSING_DIR)/quotes.c \
              $(PARSING_DIR)/tokenization.c \
              $(PARSING_DIR)/tokenization_handle.c \
              $(PARSING_DIR)/handle_heredoc.c \
              $(FREE_DIR)/free.c \
              $(SIGNALS_DIR)/signals.c \
              $(SIGNALS_DIR)/signals2.c

OBJS        = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

vg: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes \
	--suppressions=valgrind_readline.supp ./$(NAME)

.PHONY: all clean fclean re vg