/* ************************************************************************** */
/*                                                                            */
/*                                                         :::      ::::::::  */
/*   str_utils.c                                         :+:      :+:    :+:  */
/*                                                     +:+ +:+         +:+    */
/*   By: samamaev <samamaev@student.42.fr>           +#+  +:+       +#+       */
/*                                                 +#+#+#+#+#+   +#+          */
/*   Created: 2026/03/23 01:05:00 by samamaev          #+#    #+#             */
/*   										          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_str_has_dollar(const char *s)
{
	int		i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
