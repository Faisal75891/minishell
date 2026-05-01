/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 23:37:52 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/06 23:37:52 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	t_ft_putnbr_base_fd(unsigned long long nbr,
										const char *base, int fd)
{
	int		count;
	size_t	base_len;
	int		error_check;

	count = 0;
	base_len = ft_strlen(base);
	if (nbr >= base_len)
	{
		error_check = t_ft_putnbr_base_fd(nbr / base_len, base, fd);
		if (error_check == -1)
			return (-1);
		count += error_check;
	}
	error_check = ft_putchar_fdi(base[nbr % base_len], fd);
	if (error_check == -1)
		return (-1);
	count += error_check;
	return (count);
}

int	ft_putptr_fd(void *ptr, int fd)
{
	unsigned long long	num;
	int					count;

	count = 0;
	if (!ptr)
	{
		count += write(fd, "0x0", 3);
		return (count);
	}
	num = (unsigned long long)ptr;
	if (write(fd, "0x", 2) == 2)
		count += 2;
	else
		return (-1);
	count += t_ft_putnbr_base_fd(num, "0123456789abcdef", fd);
	return (count);
}
