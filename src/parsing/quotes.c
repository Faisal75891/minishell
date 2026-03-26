/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 10:24:39 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/26 10:37:33 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: quote handling
// - handle single quotes (no expansion)
// - handle double quotes (with expansion)
// - handle escaped characters

// This is used in lexer.c
int	add_word(char *command, t_lex_result *lexer, int *i)
{
	int		quote;
	char	*buffer;

	buffer = ft_strdup("");
	if(!buffer)
	{
		lexer->error = 1;
		return (0);
	}
	quote = 0;
	// loop until you find a space while unquoted
	while (command[*i])
	{
		// Check if reached criteria.
		if (quote == 0 && (ft_isspace(command[*i]) || is_operator_char(command[*i])))
			break ;
		
		// if reached quote. Flip switch
		if (command[*i] == '"')
		{
			if (quote == '"')
				quote = 0;
			else
				quote = '"';
			(*i)++;
			continue ;
		}
		if (command[*i] == '\'')
		{
			if (quote == '\'')
				quote = 0;
			else
				quote = '\'';
			(*i)++;
			continue ;
		}
		// append char
		buffer = ms_strappend_char(buffer, command[*i]);
		if (!buffer)
		{
			lexer->error = 1;
			return (0);
		}
		(*i)++;
	}
	// if quote is not 0. unclosed quote error.
	if (quote != 0)
	{
		free(buffer);
		lexer->error = 2;
		return (0);	
	}
	// create token from buffer then add it to lexer.
	append_token(lexer, create_token(TOK_WORD, Q_NONE, buffer));
	free(buffer);
	return (1);
}
