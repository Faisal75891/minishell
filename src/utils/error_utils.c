/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/26 12:53:25 by fbaras           ###   ########.fr       */
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

int	word_commit(t_lex_result *lexer, char *buffer)
{
	t_token	*token;

	token = create_token(TOK_WORD, Q_NONE, buffer);
	if (!token)
		return (word_fail(lexer, buffer, 1));
	append_token(lexer, token);
	free(buffer);
	return (1);
}



// TODO: add proper error-printing helpers later
