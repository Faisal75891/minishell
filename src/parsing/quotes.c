/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 10:24:39 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 23:39:55 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../../include/minishell.h"

// TODO: quote handling
// - handle single quotes (no expansion) ------------DONE
// - handle double quotes (with expansion) ----------DONE
// - handle escaped characters ----------------------.(Do we need this?)

static int	strip_outer_quote(char **buffer, int quote_char)
{
	size_t	len;
	char	*tmp;

	if (!buffer || !*buffer)
		return (1);
	len = ft_strlen(*buffer);
	if (len >= 2 && (*buffer)[0] == quote_char
		&& (*buffer)[len - 1] == quote_char)
	{
		tmp = ft_substr(*buffer, 1, len - 2);
		if (!tmp)
			return (0);
		free(*buffer);
		*buffer = tmp;
	}
	return (1);
}

int	get_and_strip_final_quote(char **buffer, t_quote_status *quotes,
		int *final_quote)
{
	if (quotes->is_double && !quotes->is_single && !quotes->is_unquoted)
		*final_quote = '"';
	else if (quotes->is_single && !quotes->is_double && !quotes->is_unquoted)
		*final_quote = '\'';
	else
		*final_quote = 0;
	if (*final_quote == '"' || *final_quote == '\'')
	{
		if (!strip_outer_quote(buffer, *final_quote))
		{
			ft_putstr_fd("Error: Couldn't strip quote\n", 2);
			return (0);
		}
	}
	return (1);
}

static int	scan_word_segment(const char *command,
		char **buffer, int *i, int *final_quote)
{
	t_quote_status	quotes;
	int				consumed;

	ft_memset(&quotes, 0, sizeof(t_quote_status));
	while (command[*i])
	{
		if (quotes.quote == 0
			&& (ft_isspace(command[*i])
				|| is_operator_char(command[*i])))
			break ;
		consumed = handle_quotes(buffer, command, i, &quotes);
		if (consumed == -1)
			return (-1);
		(*i) += consumed;
	}
	if (!get_and_strip_final_quote(buffer, &quotes, final_quote))
		return (-1);
	if (*final_quote == -1)
		return (-1);
	if (quotes.quote != 0)
		return (0);
	return (1);
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
	if (!balance)
		return (word_fail(lexer, buffer, 2));
	return (word_commit(lexer, buffer, final_quote));
}
