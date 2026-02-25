NAME		= m
INCLUDES	= -I./include -I./libft

SRC_DIR		= src
LIBFT_DIR	= libft

SRCS		= $(SRC_DIR)/main/main.c \
			  $(SRC_DIR)/main/init.c \
			  $(SRC_DIR)/parsing/parser.c \
			  $(SRC_DIR)/parsing/lexer.c \
			  $(SRC_DIR)/parsing/quotes.c \
			  $(SRC_DIR)/parsing/expansion.c \
			  $(SRC_DIR)/execution/executor.c \
			  $(SRC_DIR)/execution/pipes.c \
			  $(SRC_DIR)/execution/redirects.c \
			  $(SRC_DIR)/builtins/cd.c \
			  $(SRC_DIR)/builtins/echo.c \
			  $(SRC_DIR)/builtins/env.c \
			  $(SRC_DIR)/builtins/export.c \
			  $(SRC_DIR)/builtins/pwd.c \
			  $(SRC_DIR)/builtins/unset.c \
			  $(SRC_DIR)/builtins/exit.c \
			  $(SRC_DIR)/signals/signals.c \
			  $(SRC_DIR)/utils/path_utils.c \
			  $(SRC_DIR)/utils/cleanup.c \
			  $(SRC_DIR)/utils/env_utils.c \
			  $(SRC_DIR)/utils/error_utils.c \
			  $(SRC_DIR)/utils/history.c

OBJS		= $(SRCS:.c=.o)
LIBFT		= $(LIBFT_DIR)/libft.a

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
