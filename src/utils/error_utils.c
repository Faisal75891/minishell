/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/07 19:18:54 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// small generic helpers, can be reused across project
char	*ms_strappend_free(char *s1, char *s2)
{
	char	*tmp;

	if (!s2)
	{
		free(s1);
		return (NULL);
	}
	tmp = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (tmp);
}

char	*ms_strappend_char(char *s, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(s, buf);
	free(s);
	return (tmp);
}

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



// TODO: add proper error-printing helpers later
