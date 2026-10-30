# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: damalca <damalca@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/28 10:06:20 by damalca           #+#    #+#              #
#    Updated: 2026/01/15 13:06:09 by damalca          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -fPIE
AR = ar rcs
RM = rm -f

SRC = \
        Libc_functions/ft_atoi.c \
        Libc_functions/ft_bzero.c \
        Libc_functions/ft_calloc.c \
        Libc_functions/ft_isalnum.c \
        Libc_functions/ft_isalpha.c \
        Libc_functions/ft_isascii.c \
        Libc_functions/ft_isdigit.c \
        Libc_functions/ft_isprint.c \
        Libc_functions/ft_memchr.c \
        Libc_functions/ft_memcmp.c \
        Libc_functions/ft_memcpy.c \
        Libc_functions/ft_memmove.c \
        Libc_functions/ft_memset.c \
        Libc_functions/ft_strchr.c \
        Libc_functions/ft_strdup.c \
        Libc_functions/ft_strlcat.c \
        Libc_functions/ft_strlcpy.c \
        Libc_functions/ft_strlen.c \
        Libc_functions/ft_strncmp.c \
        Libc_functions/ft_strnstr.c \
        Libc_functions/ft_strrchr.c \
        Libc_functions/ft_tolower.c \
        Libc_functions/ft_toupper.c \
        Libc_functions/ft_atol.c \
        Libc_functions/ft_strcmp.c \
	Additional_functions/ft_substr.c \
	Additional_functions/ft_strjoin.c \
	Additional_functions/ft_strtrim.c \
	Additional_functions/ft_split.c \
        Additional_functions/ft_itoa.c \
        Additional_functions/ft_strmapi.c \
        Additional_functions/ft_striteri.c \
        Additional_functions/ft_putchar_fd.c \
        Additional_functions/ft_putstr_fd.c \
        Additional_functions/ft_putendl_fd.c \
        Additional_functions/ft_putnbr_fd.c \
        ft_printf/ft_printf.c \
        ft_printf/printf_utils.c \
        ft_printf/printhexa.c \
        ft_printf/printnormal.c \
        get_next_line/get_next_line.c \
        get_next_line/get_next_line_utils.c \
        

BONUS_SRC = \
        Chained_lists/ft_lstnew_bonus.c \
        Chained_lists/ft_lstadd_front_bonus.c \
        Chained_lists/ft_lstsize_bonus.c \
        Chained_lists/ft_lstlast_bonus.c \
        Chained_lists/ft_lstadd_back_bonus.c \
        Chained_lists/ft_lstdelone_bonus.c \
        Chained_lists/ft_lstclear_bonus.c \
        Chained_lists/ft_lstiter_bonus.c \
        Chained_lists/ft_lstmap_bonus.c \

OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(OBJ) $(BONUS_OBJ)
	$(AR) $(NAME) $(OBJ) $(BONUS_OBJ)

clean:
	$(RM) $(OBJ) $(BONUS_OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
