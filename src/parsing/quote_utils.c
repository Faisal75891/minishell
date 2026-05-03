/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 23:37:12 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 23:42:10 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// returns chars consumed
static int	backslash_in_squote(char **buffer, const char *command, int *i)
{
	int	consumed;

	consumed = 0;
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
		consumed = 1;
	}
	return (consumed);
}

// returns chars consumed
static int	backslash_in_dquote(char **buffer, const char *command,
		int *i, t_quote_status *quotes)
{
	int	new_quote;

	new_quote = is_quote(command[*i], quotes->quote);
	quotes->quote = new_quote;
	if ((command[*i] == '"' || command[*i] == '\'')
		&& new_quote != quotes->prev_quote)
	{
		if (command[*i] == '"')
			quotes->is_double = 1;
		else
			quotes->is_single = 1;
	}
	else
	{
		if (quotes->quote == 0)
			quotes->is_unquoted = 1;
		else if (quotes->quote == '"')
			quotes->is_double = 1;
		else if (quotes->quote == '\'')
			quotes->is_single = 1;
	}
	*buffer = ms_strappend_char(*buffer, command[*i]);
	if (!*buffer)
		return (-1);
	return (1);
}

int	handle_quotes(char **buffer, const char *command,
		int *i, t_quote_status *quotes)
{
	int	consumed;

	consumed = 0;
	if (command[*i] == '\\' && quotes->quote != '\'')
	{
		consumed = backslash_in_squote(buffer, command, i);
		if (consumed == -1)
			return (-1);
	}
	else
	{
		quotes->prev_quote = quotes->quote;
		consumed = backslash_in_dquote(buffer, command, i, quotes);
		if (consumed == -1)
			return (-1);
	}
	return (consumed);
}

int	set_mode(const char	*s, int *i, int mode)
{
	if (mode == 0 && s[*i] == '\'')
	{
		(*i)++;
		return (1);
	}
	if (mode == 1 && s[*i] == '\'')
	{
		(*i)++;
		return (0);
	}
	if (mode == 0 && s[*i] == '"')
	{
		(*i)++;
		return (2);
	}
	if (mode == 2 && s[*i] == '"')
	{
		(*i)++;
		return (0);
	}
	return (mode);
}
