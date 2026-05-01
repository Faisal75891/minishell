/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fdi.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 22:40:42 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/06 22:40:42 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar_fdi(char c, int fd)
{
	int	error_check;

	error_check = write(fd, &c, 1);
	if (error_check < 0)
		return (-1);
	return (1);
}
