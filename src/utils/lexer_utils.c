/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 10:21:25 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/26 10:21:25 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_here_doc(t_lex_result *lexer)
{
	t_token	*token;

	token = create_token(TOK_HEREDOC, Q_NONE, "<<");
	if (!token)
	{
		lexer->error = 1;
		return ;
	}
	append_token(lexer, token);
}

void	add_append(t_lex_result *lexer)
{
	t_token	*token;

	token = create_token(TOK_REDIR_APPEND, Q_NONE, ">>");
	if (!token)
	{
		lexer->error = 1;
		return ;
	}
	append_token(lexer, token);
}

void	add_pipe(t_lex_result *lexer)
{
	t_token	*token;

	token = create_token(TOK_PIPE, Q_NONE, "|");
	if (!token)
	{
		lexer->error = 1;
		return ;
	}
	append_token(lexer, token);
}

void	add_redirect_out(t_lex_result *lexer)
{
	t_token	*token;

	token = create_token(TOK_REDIR_OUT, Q_NONE, ">");
	if (!token)
	{
		lexer->error = 1;
		return ;
	}
	append_token(lexer, token);
}

void	add_redirect_in(t_lex_result *lexer)
{
	t_token	*token;

	token = create_token(TOK_REDIR_IN, Q_NONE, "<");
	if (!token)
	{
		lexer->error = 1;
		return ;
	}
	append_token(lexer, token);
}
