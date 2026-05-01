/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 20:17:13 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/08 20:17:13 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*gnl_strjoin(char const *s1, char const *s2)
{
	char	*string;
	int		len;
	char	*temp;

	if (!s1)
		return (gnl_strdup(s2));
	if (!s2)
		return (gnl_strdup(s1));
	len = gnl_strlen(s1) + gnl_strlen(s2) + 1;
	string = malloc(len);
	if (!string)
		return (NULL);
	temp = string;
	while (*s1)
		*temp++ = *s1++;
	while (*s2)
		*temp++ = *s2++;
	*temp = '\0';
	return (string);
}

char	*gnl_strchr(char *s, int c)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

char	*gnl_strdup(const char *s)
{
	char	*string;
	int		i;
	int		len;

	if (!s)
		return (NULL);
	len = gnl_strlen(s);
	string = malloc(len + 1);
	if (!string)
		return (NULL);
	i = 0;
	while (i < len)
	{
		string[i] = s[i];
		i++;
	}
	string[i] = '\0';
	return (string);
}
