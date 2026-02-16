NAME = m
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

S = minishell.c  read_commands.c  utils.c  history.c
O = $(S:.c=.o)

LIBFT = ./libft/libft.a

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIBFT) $(O)
	$(CC) $(CFLAGS) $(O) $(LIBFT) -o $(NAME)


$(LIBFT):
	make -C ./libft

clean:
	make clean -C ./libft
	rm -rf $(O)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONEY: all clean fclean re
