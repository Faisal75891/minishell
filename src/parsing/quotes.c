/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 10:24:39 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/26 12:54:08 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: quote handling
// - handle single quotes (no expansion) ------------DONE
// - handle double quotes (with expansion) ----------DONE
// - handle escaped characters ----------------------.(Do we need this?)

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

static int	is_quote(char c, int quote)
{
	if (c == '"' && quote != '\'')
	{
		if (quote == '"')
			return (0);
		return ('"');
	}
	if (c == '\'' && quote != '"')
	{
		if (quote == '\'')
			return (0);
		return ('\'');
	}
	return (quote);
}

static int	scan_word_segment(char *command, char **buffer, int *i)
{
	int	quote;
	int	new_quote;

	quote = 0;
	while (command[*i])
	{
		if (quote == 0
			&& (ft_isspace(command[*i])
				|| is_operator_char(command[*i])))
			break ;
		new_quote = is_quote(command[*i], quote);
		if (new_quote != quote)
		{
			quote = new_quote;
			(*i)++;
			continue ;
		}
		*buffer = ms_strappend_char(*buffer, command[*i]);
		if (!*buffer)
			return (-1);
		(*i)++;
	}
	return (quote);
}

// This is used in lexer.c
int	add_word(char *command, t_lex_result *lexer, int *i)
{
	int		quote;
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (word_fail(lexer, NULL, 1));
	quote = scan_word_segment(command, &buffer, i);
	if (quote < 0)
		return (word_fail(lexer, buffer, 1));
	if (quote != 0)
		return (word_fail(lexer, buffer, 2));
	return (word_commit(lexer, buffer));
}
