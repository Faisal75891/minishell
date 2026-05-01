/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 00:33:19 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../../include/minishell.h"

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

char	*handle_dollar_seq(const char *s, int *i, t_shell *shell)
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
			res = append_char(res, s, &i, shell);
		else
		{
			mode = set_mode(s, &i);
			if (mode != 1)
				res = append_char(res, s, &i, shell);
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
