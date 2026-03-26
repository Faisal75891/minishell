/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:10:55 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/26 11:05:16 by fbaras           ###   ########.fr       */
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

t_token	*create_token(t_token_type type, t_quote_type quote, char *word)
{
	t_token	*token;

	token = malloc (sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->quote = quote;
	token->word = ft_strdup(word);
	token->next = NULL;
	return (token);
}

void	append_token(t_lex_result *lexer, t_token *token)
{
	if (lexer->count == 0)
	{
		lexer->head = token;
		lexer->tail = token;
		lexer->head->next = NULL;
	}
	else if (lexer->count == 1)
	{
		lexer->tail = token;
		lexer->head->next = lexer->tail;
	}
	else
	{
		lexer->tail->next = token;
		lexer->tail = token;
	}
	lexer->count++;
}

// example command: cat << EOF > file.txt
// output: WORD(cat) HERE_DOC(<<) WORD(EOF) redirect(>) WORD(file.txt)
void	tokenize_lexer(char *command, t_lex_result *lexer)
{
	int	i;

	i = 0;
	while (command[i])
	{
		if (ft_isspace(command[i]))
		{
			i++;
			continue;
		}
		if (command[i] == '<' && command[i + 1] && command[i + 1] == '<')
		{
			add_here_doc(lexer);
			i += 2;
			continue;
		}
		if (command[i] == '>' && command[i + 1] && command[i + 1] == '>')
		{
			add_append(lexer);
			i += 2;
			continue;
		}
		if (command[i] == '|')
		{
			add_pipe(lexer);
			i++;
			continue ;
		}
		if (command[i] == '>')
		{
			add_redirect_out(lexer);
			i++;
			continue;
		}
		if (command[i] == '<')
		{
			add_redirect_in(lexer);
			i++;
			continue;
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
