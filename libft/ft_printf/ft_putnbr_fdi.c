/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbrfd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 22:30:36 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/06 22:30:36 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr_fdi(int n, int i, int fd)
{
	long	num;
	int		error_check;

	num = n;
	if (num < 0)
	{
		error_check = ft_putchar_fdi('-', fd);
		if (error_check == -1)
			return (-1);
		i += error_check;
		num = -num;
	}
	if (num > 9)
	{
		error_check = ft_putnbr_fdi(num / 10, 0, fd);
		if (error_check == -1)
			return (-1);
		i += error_check;
	}
	error_check = ft_putchar_fdi(num % 10 + '0', fd);
	if (error_check == -1)
		return (-1);
	i += error_check;
	return (i);
}
