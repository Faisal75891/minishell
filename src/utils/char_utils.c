/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:38:58 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 00:32:40 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../../include/minishell.h"

// first != 0 -> first char of variable name, else subsequent char
int	ms_is_var_char(int c, int first)
{
	if (first)
		return (ft_isalpha((unsigned char)c) || c == '_');
	return (ft_isalnum((unsigned char)c) || c == '_');
}

int	set_mode(const char	*s, int *i)
{
	int	mode;

	if (mode == 0 && s[*i] == '\'')
	{
		mode = 1;
		(*i)++;
	}
	if (mode == 1 && s[*i] == '\'')
	{
		mode = 0;
		(*i)++;
	}
	if (mode == 0 && s[*i] == '"')
	{
		mode = 2;
		(*i)++;
	}
	if (mode == 2 && s[*i] == '"')
	{
		mode = 0;
		(*i)++;
	}
	return (mode);
}

char	*append_char(char *res, const char *s, int *i, t_shell *shell)
{
	char	*to_add;

	if (s[*i] == '$')
	{
		to_add = handle_dollar_seq(s, i, shell);
		res = ms_strappend_free(res, to_add);
	}
	else
	{
		res = ms_strappend_char(res, s[*i]);
		(*i)++;
	}
	return (res);
}
