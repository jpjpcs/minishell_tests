# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/03 10:15:08 by wcorrea-          #+#    #+#              #
#    Updated: 2024/05/08 14:14:54 by joaosilva        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = src/main.c src/envp/envp.c  src/envp/envp_utils.c  src/envp/envp_utils2.c src/check_args.c #\
		src/environment.c \
		src/input.c \
		src/parse.c \
		src/signals.c \
		src/redirections.c src/heredoc.c \
		src/tokens.c src/tokens_utils.c src/tokens_split.c \
		src/commands_manager.c src/commands_execute.c src/commands_utils.c \
		src/builtins_basic.c src/builtins_complex.c src/builtins_utils.c \
		src/builtins_export_lists.c src/builtins_export_tmp.c \
		src/utils.c src/utils_extra.c

OBJS = ${SRC:.c=.o}

CC = clang
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
INCLUDE = -I includes
MAKE = make -C
LIBFT_PATH = libft
LFLAGS = -L ./Libft_obj -lft -lreadline

.c.o:
		${CC} ${CFLAGS} ${INCLUDE} -c $< -o ${<:.c=.o}

all:	${NAME}

$(NAME): ${OBJS}
		${MAKE} ${LIBFT_PATH}
		${CC} ${CFLAGS} ${INCLUDE} ${OBJS} ${LFLAGS} -o ${NAME}

clean:
		${MAKE} ${LIBFT_PATH} clean
		${RM} ${OBJS}

fclean: clean
		${MAKE} ${LIBFT_PATH} fclean
		${RM} ${NAME}

re: fclean all

leaks: readline.supp
	valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --log-file=output.log ./minishell

readline.supp:
	echo "{" > readline.supp
	echo "    leak readline" >> readline.supp
	echo "    Memcheck:Leak" >> readline.supp
	echo "    ..." >> readline.supp
	echo "    fun:readline" >> readline.supp
	echo "}" >> readline.supp
	echo "{" >> readline.supp
	echo "    leak add_history" >> readline.supp
	echo "    Memcheck:Leak" >> readline.supp
	echo "    ..." >> readline.supp
	echo "    fun:add_history" >> readline.supp
	echo "}" >> readline.supp

.SILENT:

.PHONY: all clean fclean re

