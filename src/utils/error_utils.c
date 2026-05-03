/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 02:11:09 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word_fail(t_lex_result *lexer, char *buffer, int err)
{
	if (buffer)
		free(buffer);
	lexer->error = err;
	return (0);
}

int	word_commit(t_lex_result *lexer, char *buffer, int quote)
{
	t_token	*token;
	int		real_quote;

	if (quote == '"')
		real_quote = Q_DOUBLE;
	else if (quote == '\'')
		real_quote = Q_SINGLE;
	else
		real_quote = Q_NONE;
	token = create_token(TOK_WORD, real_quote, buffer);
	if (!token)
		return (word_fail(lexer, buffer, 1));
	append_token(lexer, token);
	free(buffer);
	return (1);
}

void	exit_error(char *command)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(127);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	exit(1);
}

void	handle_error(t_lex_result *lexer)
{
	if (!lexer || lexer->error == 0)
		return ;
	if (lexer->error == 2)
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
	else if (lexer->error == 1)
		ft_putendl_fd("minishell: allocation error", 2);
	else if (lexer->error == 3)
	{
		if (!lexer->unexpected_token)
			lexer->unexpected_token = "newline";
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(lexer->unexpected_token, 2);
		ft_putendl_fd("'", 2);
	}
}
