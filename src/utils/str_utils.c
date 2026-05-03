/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 02:11:46 by samamaev          #+#    #+#             */
/*   Updated: 2026/05/02 02:11:51 by samamaev         ###   ########.fr       */
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
