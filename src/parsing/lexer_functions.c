/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 23:39:38 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 23:40:01 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
