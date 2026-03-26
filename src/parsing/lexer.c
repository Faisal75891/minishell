/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:10:55 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/26 12:44:39 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lex_result	*init_lexer(void)
{
	t_lex_result	*lex;

	lex = malloc (sizeof(t_lex_result));
	if (!lex)
		return (NULL);
	lex->count = 0;
	lex->error = 0;
	lex->head = NULL;
	lex->tail = NULL;
	return (lex);
}

static int	handle_operator(char *command, t_lex_result *lexer, int index)
{
	int	i;

	i = index;
	if (command[i] == '<' && command[i + 1] && command[i + 1] == '<')
	{
		add_here_doc(lexer);
		i += 2;
	}
	if (command[i] == '>' && command[i + 1] && command[i + 1] == '>')
	{
		add_append(lexer);
		i += 2;
	}
	if (command[i] == '|')
		return (add_pipe(lexer), i + 1);
	if (command[i] == '>')
		return (add_redirect_out(lexer), i + 1);
	if (command[i] == '<')
		return (add_redirect_in(lexer), i + 1);
	return (i);
}

// example command: cat << EOF > file.txt
// output: WORD(cat) HERE_DOC(<<) WORD(EOF) redirect(>) WORD(file.txt)
void	tokenize_lexer(char *command, t_lex_result *lexer)
{
	int	i;
	int	new_i;

	i = 0;
	while (command[i])
	{
		if (ft_isspace(command[i]))
		{
			i++;
			continue ;
		}
		new_i = handle_operator(command, lexer, i);
		if (new_i != i)
		{
			i = new_i;
			continue ;
		}
		if (!add_word(command, lexer, &i))
			return ;
	}
}

void	clear_lexer(t_lex_result *lexer)
{
	t_token	*current;
	t_token	*next;

	next = NULL;
	current = lexer->head;
	while (current)
	{
		next = current->next;
		free(current->word);
		free(current);
		current = next;
	}
	lexer->count = 0;
	lexer->error = 0;
	lexer->head = NULL;
	lexer->tail = NULL;
}
