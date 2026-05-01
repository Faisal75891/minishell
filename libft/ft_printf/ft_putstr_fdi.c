/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fdi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 22:35:10 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/06 22:35:10 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putstr_fdi(const char *s, int fd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!s)
		return (ft_putstr_fdi("(null)", 1));
	while (s[i])
	{
		if (ft_putchar_fdi(s[i], fd) > 0)
			count += 1;
		else
			return (-1);
		i++;
	}
	return (count);
}
