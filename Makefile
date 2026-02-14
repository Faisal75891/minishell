NAME = m
CC = cc
CFLAGS = -Wall -Wextra -Werror

S = minishell.c
O = $(S:.c=.o)

LIBFT = ./libft/libft.a

all: $(NAME)

$(NAME): $(O) $(LIBFT)
	$(CC) $(CFLAGS) $(O) $(LIBFT) -o $(NAME)

%.c: %.o
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C ./libft

clean:
	make clean -C ./libft
	rm -rf $(O)

fclean: $(clean)
	rm -rf $(NAME)

re: $(fclean) $(all)

.PHONEY: all clean fclean re
