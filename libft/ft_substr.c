/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:47:07 by fbaras            #+#    #+#             */
/*   Updated: 2025/07/17 13:47:07 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*string;
	unsigned int	i;
	size_t			s_len;
	size_t			actual_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start > s_len)
		return (ft_strdup(""));
	actual_len = s_len - start;
	if (len > actual_len)
		len = actual_len;
	string = malloc (len + 1);
	if (!string)
		return (NULL);
	i = 0;
	while (s[start + i] && i < len)
	{
		string[i] = s[start + i];
		i++;
	}
	string[i] = '\0';
	return (string);
}
