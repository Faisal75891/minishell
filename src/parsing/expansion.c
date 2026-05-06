/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 02:26:57 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_step(const char *s, int *i, char *res, t_shell *shell)
{
	static int	mode;

	mode = set_mode(s, i, mode);
	if (mode == 1)
	{
		if (s[*i] && s[*i] != '\'')
		{
			res = ms_strappend_char(res, s[*i]);
			(*i)++;
		}
		return (res);
	}
	return (append_char(res, s, i, shell));
}

// I will try adding the quote state from the parser
static char	*expand_loop(const char *s, t_quote_type quote, t_shell *shell)
{
	int		i;
	char	*res;
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
			res = append_char(res, s, &i, shell);
			if (!res)
				return (NULL);
			continue ;
		}
		res = expand_step(s, &i, res, shell);
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
