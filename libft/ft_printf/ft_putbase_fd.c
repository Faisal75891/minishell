/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putbase.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:57:15 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/06 21:57:15 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr_base_fd(unsigned int nbr, const char *base, int fd)
{
	int	i;
	int	error_check;

	i = 0;
	if (nbr >= ft_strlen(base))
	{
		error_check = ft_putnbr_base_fd(nbr / ft_strlen(base), base, fd);
		if (error_check == -1)
			return (-1);
		i += error_check;
	}
	error_check = ft_putchar_fdi(base[nbr % ft_strlen(base)], fd);
	if (error_check == -1)
		return (-1);
	i += error_check;
	return (i);
}
