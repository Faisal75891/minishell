/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 14:52:20 by fbaras            #+#    #+#             */
/*   Updated: 2025/07/17 14:52:20 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	count_words(const char *s, char c)
{
	int	i;
	int	in_word;

	i = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			i++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (i);
}

static void	*free_array(char **arr, int size)
{
	while (size--)
		free(arr[size]);
	free(arr);
	return (NULL);
}

static int	fill_array(char **arr, const char *s, char c)
{
	int	i;
	int	len;

	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			len = 0;
			while (s[len] && s[len] != c)
				len++;
			arr[i] = malloc(len + 1);
			if (!arr[i])
				return (i);
			ft_strlcpy(arr[i++], s, len + 1);
			s += len;
		}
		else
			s++;
	}
	arr[i] = NULL;
	return (-1);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		fail_index;

	if (!s)
		return (NULL);
	arr = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	fail_index = fill_array(arr, s, c);
	if (fail_index >= 0)
		return (free_array(arr, fail_index));
	return (arr);
}
