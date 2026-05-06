/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:38:58 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 23:21:41 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// first != 0 -> first char of variable name, else subsequent char
int	ms_is_var_char(int c, int first)
{
	if (first)
		return (ft_isalpha((unsigned char)c) || c == '_');
	return (ft_isalnum((unsigned char)c) || c == '_');
}

char	*handle_tilde(const char *s, int *i, t_shell *shell)
{
	char	*home;
	char	*home_env;

	(*i)++;
	if (!s[*i] || s[*i] == '/')
	{
		home_env = get_env_value("HOME", shell->env);
		home = ft_strdup(home_env);
	}
	else
		return (ft_strdup("~"));
	return (home);
}

// i didn't know where to put this lol
int	split_len(char **arr)
{
	int	len;

	len = 0;
	if (!arr)
		return (0);
	while (arr[len])
		len++;
	return (len);
}
