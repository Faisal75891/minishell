/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:10:55 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 00:58:24 by fbaras           ###   ########.fr       */
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
	lex->unexpected_token = NULL;
	return (lex);
}

static int	handle_operator(const char *command, t_lex_result *lexer, int index)
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

static int	check_operator_word(t_lex_result *lexer, const char *command, int i)
{
	while (command[i] && ft_isspace(command[i]))
		i++;
	if (!command[i])
	{
		lexer->error = 3;
		lexer->unexpected_token = ft_strdup("newline");
		return (0);
	}
	if (is_operator_char(command[i]))
	{
		lexer->error = 3;
		lexer->unexpected_token = ft_substr(command, i, 1);
		return (0);
	}
	return (1);
}

static int	check_last_token(t_lex_result *lexer)
{
	if (!lexer || !lexer->head)
		return (1);
	if (lexer->head->type == TOK_PIPE)
	{
		lexer->error = 3;
		lexer->unexpected_token = ft_strdup("|");
		return (0);
	}
	if (!lexer->unexpected_token && lexer->tail->type != TOK_WORD)
	{
		lexer->error = 3;
		lexer->unexpected_token = ft_strdup("newline");
		return (0);
	}
	return (1);
}

// example command: cat << EOF > file.txt
// output: WORD(cat) HERE_DOC(<<) WORD(EOF) redirect(>) WORD(file.txt)
void	tokenize_lexer(const char *command, t_lex_result *lexer)
{
	int	i;
	int	new_i;

	i = 0;
	while (command[i] && lexer->error == 0)
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
			if (!check_operator_word(lexer, command, i))
				break ;
			continue ;
		}
		if (!add_word(command, lexer, &i))
			break ;
	}
	check_last_token(lexer);
	if (lexer->error > 0)
		handle_error(lexer);
}
