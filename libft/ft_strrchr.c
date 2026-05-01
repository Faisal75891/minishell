/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:38:25 by fbaras            #+#    #+#             */
/*   Updated: 2025/07/15 18:38:25 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*temp_ptr;

	temp_ptr = NULL;
	while (*s)
	{
		if (*s == (char)c)
			temp_ptr = (char *)s;
		s++;
	}
	if (*s == (char)c)
		temp_ptr = (char *)s;
	return (temp_ptr);
}
