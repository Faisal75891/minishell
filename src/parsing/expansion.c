/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/11 20:08:24 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_special_vars(const char *s, int *i, t_shell *shell)
{
	if (s[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->last_status));
	}
	if (s[*i] == '$')
	{
		(*i)++;
		return (ft_itoa((int)getpid()));
	}
	return (NULL);
}

static char	*handle_named_var(const char *s, int *i, t_shell *shell)
{
	int		start;
	char	*name;
	char	*value;

	if (!ms_is_var_char((unsigned char)s[*i], 1))
		return (NULL);
	start = *i;
	while (s[*i] && ms_is_var_char((unsigned char)s[*i], 0))
		(*i)++;
	name = ft_substr(s, start, *i - start);
	if (!name)
		return (NULL);
	value = get_env_value(name, shell->env);
	free(name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static char	*handle_dollar_seq(const char *s, int *i, t_shell *shell)
{
	char	*res;

	(*i)++;
	res = handle_special_vars(s, i, shell);
	if (res)
		return (res);
	res = handle_named_var(s, i, shell);
	if (res)
		return (res);
	return (ft_substr(s, *i - 1, 2));
}

// I will try adding the quote state from the parser
static char	*expand_loop(const char *s, t_quote_type quote, t_shell *shell)
{
	int		i;
	char	*res;
	char	*to_add;
	int		mode;

	mode = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (quote == Q_DOUBLE)
		{
			if (s[i] == '$')
			{
				to_add = handle_dollar_seq(s, &i, shell);
				res = ms_strappend_free(res, to_add);
			}
			else
			{
				res = ms_strappend_char(res, s[i]);
				i++;
			}
		}
		else
		{
			if (mode == 0 && s[i] == '\'') { mode = 1; i++; continue; }
			if (mode == 1 && s[i] == '\'') { mode = 0; i++; continue; }
			if (mode == 0 && s[i] == '"')  { mode = 2; i++; continue; }
			if (mode == 2 && s[i] == '"')  { mode = 0; i++; continue; }

			if (s[i] == '$' && mode != 1)
			{
				to_add = handle_dollar_seq(s, &i, shell);
				res = ms_strappend_free(res, to_add);
			}
			else
			{
				res = ms_strappend_char(res, s[i]);
				i++;
			}
		}

		if (!res)
			return (NULL);
	}
	return (res);
}

// Expand $VAR, $? and $$ in a single argument string.
char	*expand_variables(const char *s, t_quote_type quote, t_shell *shell)
{
	if (!s || !shell)
		return (NULL);
	if (quote == Q_SINGLE)
		return (ft_strdup(s));
	if (quote == Q_DOUBLE && !ms_str_has_dollar(s))
		return (ft_strdup(s));
	return (expand_loop(s, quote, shell));
}
