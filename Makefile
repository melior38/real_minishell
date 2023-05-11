# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lcamilo- <lcamilo-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/11 16:47:09 by lcamilo-          #+#    #+#              #
#    Updated: 2022/10/19 20:35:17 by lcamilo-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# ------------  COLORS  ------------------------------------------------------ #
COLOUR_GREEN=\033[0;32m
COLOUR_RED=\033[0;31m
COLOUR_ORANGE=\033[0;34m
COLOUR_END=\033[0m
COLOUR_YELLOW=\033[0;33m
COLOUR_CYAN=\033[0;36m

# ------------  PROJECT  ----------------------------------------------------- #
NAME		= minishell

# ------------  DIRECTORIES  ------------------------------------------------- #
LIB				= libftprintfgnl
SRC_PATH		= src/
HEADERS			= includes
HEADERS_LIB		= libftprintfgnl/includes

INC			= $(addprefix $HEADERS_LIB)

# ------------  FLAGS  ------------------------------------------------------- #
CFLAGS 		= -Wall -Wextra -Werror -fsanitize=address -I $(HEADERS) -I $(HEADERS_LIB)
CC			= @cc
RM			= rm -rf

# ------------  SOURCE FILES  ------------------------------------------------ #
SRC 		= 	main.c							\
				builtin/echo.c					\
				builtin/cd.c					\
				builtin/pwd.c					\
				builtin/export.c				\
				builtin/unset.c					\
				builtin/env.c					\
				builtin/exit.c					\
				execution/execution.c			\
				parsing/parsing.c				\
				parsing/quotes.c				\
				parsing/space.c					\
				parsing/dollars.c				\
				parsing/get_cmd.c				\
				parsing/here_doc.c				\
				utils/utils_lst.c				\
				utils/utils_lst2.c				\
				utils/exec_utils.c				\
				utils/redir_utils.c				\
				utils/utils.c					\
				utils/get_env.c					\
				utils/sig.c						\
				free/free.c

# ------------  FILEPATHS  --------------------------------------------------- #
OBJS				= ${SRC:.c=.o}
OBJS_DIR			= objs
DIRS				= objs objs/utils objs/parsing objs/execution objs/builtin objs/free
_OBJS				= $(addprefix $(OBJS_DIR)/, $(OBJS))

all: lib $(NAME)

lib:
			@echo "$(COLOUR_CYAN)Compilation libft..."
			make -C $(LIB)
			@echo "$(COLOUR_YELLOW)Deplacement du libft.a $(COLOUR_END)"
			@cp ./libftprintfgnl/libft.a libft.a

$(NAME): $(_OBJS)
			@echo "$(COLOUR_CYAN)Compile..."
			$(CC) $(_OBJS) libft.a $(CFLAGS) -o $@ -L libftprintfgnl -lreadline -lft -I $(HOME)/.brew/opt/readline/include -L $(HOME)/.brew/opt/readline/lib
			@echo "$(COLOUR_RED)Minishell ready. $(COLOUR_END)"

$(OBJS_DIR)/%.o: $(SRC_PATH)/%.c
		@mkdir -p $(DIRS)
		${CC} $(CFLAGS) -c $< -o $@ -g3

clean:
			$(RM) ${OBJS}
			rm -rf $(OBJS_DIR)
			make fclean -C $(LIB)

fclean:		clean
			$(RM) $(NAME) *.a

re:			fclean all

.PHONY: re ignore fclean clean all $(LIBFT) $(NAME) $(PRINTF)