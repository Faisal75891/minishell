/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:40:07 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/06 21:40:07 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putunbr_fd(unsigned int nbr, int fd)
{
	char	digit;
	int		i;
	int		error_check;

	i = 0;
	if (nbr > 9)
	{
		error_check = ft_putunbr_fd(nbr / 10, fd);
		if (error_check == -1)
			return (-1);
		i += error_check;
	}
	digit = nbr % 10 + '0';
	error_check = ft_putchar_fdi(digit, 1);
	if (error_check == -1)
		return (-1);
	i += error_check;
	return (i);
}
