# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: damalca <damalca@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/18 11:51:14 by damalca           #+#    #+#              #
#    Updated: 2025/08/25 15:12:12 by damalca          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror

MANDATORY_DIR = mandatory
BONUS_DIR = bonus
LIBFT_DIR = Libft

LIBFT = $(LIBFT_DIR)/libft.a

MANDATORY_SERVER = server
MANDATORY_CLIENT = client
BONUS_SERVER = server_bonus
BONUS_CLIENT = client_bonus

SERVER_SRCS	= $(MANDATORY_DIR)/server.c
CLIENT_SRCS	= $(MANDATORY_DIR)/client.c

SERVER_BONUS_SRCS = $(BONUS_DIR)/server_bonus.c
CLIENT_BONUS_SRCS = $(BONUS_DIR)/client_bonus.c

SERVER_OBJS	= $(SERVER_SRCS:.c=.o)
CLIENT_OBJS	= $(CLIENT_SRCS:.c=.o)
SERVER_BONUS_OBJS = $(SERVER_BONUS_SRCS:.c=.o)
CLIENT_BONUS_OBJS = $(CLIENT_BONUS_SRCS:.c=.o)

all: $(MANDATORY_SERVER) $(MANDATORY_CLIENT)

bonus: all $(BONUS_SERVER) $(BONUS_CLIENT)

$(MANDATORY_SERVER): $(LIBFT) $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJS) $(LIBFT)

$(MANDATORY_CLIENT): $(LIBFT) $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJS) $(LIBFT)

$(BONUS_SERVER): $(LIBFT) $(SERVER_BONUS_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SERVER_BONUS_OBJS) $(LIBFT)

$(BONUS_CLIENT): $(LIBFT) $(CLIENT_BONUS_OBJS)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_BONUS_OBJS) $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_BONUS_OBJS) $(CLIENT_BONUS_OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(MANDATORY_SERVER) $(MANDATORY_CLIENT) $(BONUS_SERVER) $(BONUS_CLIENT)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean fclean re