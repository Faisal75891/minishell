/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 10:24:39 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/01 23:47:51 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../../include/minishell.h"

// TODO: quote handling
// - handle single quotes (no expansion) ------------DONE
// - handle double quotes (with expansion) ----------DONE
// - handle escaped characters ----------------------.(Do we need this?)

t_token	*create_token(t_token_type type, t_quote_type quote, const char *word)
{
	t_token	*token;

	token = malloc (sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->quote = quote;
	token->word = ft_strdup(word);
	if (!token->word)
	{
		free(token);
		return (NULL);
	}
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

static int	strip_outer_quote(char **buffer, int quote_char)
{
	size_t	len;
	char	*tmp;

	if (!buffer || !*buffer)
		return (1);
	len = ft_strlen(*buffer);
	if (len >= 2 && (*buffer)[0] == quote_char && (*buffer)[len - 1] == quote_char)
	{
		tmp = ft_substr(*buffer, 1, len - 2);
		if (!tmp)
			return (0);
		free(*buffer);
		*buffer = tmp;
	}
	return (1);
}

static int	scan_word_segment(const char *command, char **buffer, int *i, int *final_quote)
{
	int	quote;
	int	consumed;
	int	is_unquoted;
	int	is_double;
	int	is_single;
	int	prev_quote;
	int	new_quote;

	quote = 0;
	is_double = 0;
	is_single = 0;
	is_unquoted = 0;
	while (command[*i])
	{
		consumed = 1;
		if (quote == 0
			&& (ft_isspace(command[*i])
				|| is_operator_char(command[*i])))
			break ;
		if (command[*i] == '\\' && quote != '\'')
		{
			if (command[*i + 1])
			{
				*buffer = ms_strappend_char(*buffer, command[*i + 1]);
				if (!*buffer)
					return (-1);
				consumed = 2;
			}
			else
			{
				*buffer = ms_strappend_char(*buffer, command[*i]);
				if (!*buffer)
					return (-1);
			}
		}
		else
		{
			prev_quote = quote;
			new_quote = is_quote(command[*i], quote);
			quote = new_quote;
			if ((command[*i] == '"' || command[*i] == '\'') && new_quote != prev_quote)
			{
				if (command[*i] == '"')
					is_double = 1;
				else
					is_single = 1;
			}
			else
			{
				if (quote == 0)
					is_unquoted = 1;
				else if (quote == '"')
					is_double = 1;
				else if (quote == '\'')
					is_single = 1;
			}
			*buffer = ms_strappend_char(*buffer, command[*i]);
			if (!*buffer)
				return (-1);
		}
		(*i) += consumed;
	}
	if (quote != 0)
		return (1);
	if (is_double && !is_single && !is_unquoted)
		*final_quote = '"';
	else if (is_single && !is_double && !is_unquoted)
		*final_quote = '\'';
	else
		*final_quote = 0;
	if (*final_quote == '"' || *final_quote == '\'')
	{
		if (!strip_outer_quote(buffer, *final_quote))
		 	return (-1);
	}
	return (0);
}

// This is used in lexer.c
int	add_word(const char *command, t_lex_result *lexer, int *i)
{
	int		balance;
	int		final_quote;
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (word_fail(lexer, NULL, 1));
	balance = scan_word_segment(command, &buffer, i, &final_quote);
	if (balance < 0)
		return (word_fail(lexer, buffer, 1));
	if (balance != 0)
		return (word_fail(lexer, buffer, 2));
	return (word_commit(lexer, buffer, final_quote));
}
