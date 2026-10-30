# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mchiacha <mchiacha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/20 12:09:55 by damalca           #+#    #+#              #
#    Updated: 2026/03/16 11:47:55 by mchiacha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = cub3d

SRCS = $(shell find ./srcs -name "*.c")

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

MLX_DIR = ./minilibx
LIBFT_DIR = ./Libft
LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx.a
MLXFLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz -lXfixes

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	make -C $(MLX_DIR)
	$(CC) $(OBJS) $(MLX) $(MLXFLAGS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	make clean -C $(MLX_DIR)
	make -C $(LIBFT_DIR) fclean

fclean: clean
	$(RM) $(NAME)
	make clean -C $(MLX_DIR)
	make -C $(LIBFT_DIR) fclean

re: fclean all

norm:
	norminette ./includes ./srcs ./Libft

.PHONY: all clean fclean re norm
