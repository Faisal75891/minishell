/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 17:33:55 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/05 19:52:30 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	handle_specifier(const char format, va_list ap)
{
	if (format == 'd' || format == 'i')
		return (ft_putnbr_fdi(va_arg(ap, signed int), 0, 1));
	else if (format == 'u')
		return (ft_putunbr_fd(va_arg(ap, unsigned int), 1));
	else if (format == 'x')
		return (ft_putnbr_base_fd(va_arg(ap, unsigned int),
				"0123456789abcdef", 1));
	else if (format == 'X')
		return (ft_putnbr_base_fd(va_arg(ap, unsigned int),
				"0123456789ABCDEF", 1));
	else if (format == 'p')
		return (ft_putptr_fd(va_arg(ap, void *), 1));
	else if (format == 's')
		return (ft_putstr_fdi(va_arg(ap, char *), 1));
	else if (format == 'c')
		return (ft_putchar_fdi(va_arg(ap, int), 1));
	else if (format == '%')
		return (ft_putchar_fdi('%', 1));
	else
		return (ft_putchar_fdi(format, 1));
	return (-1);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		chars_written;

	va_start(ap, format);
	chars_written = 0;
	while (*format)
	{
		if (*format == '%' && ++format)
		{
			chars_written += handle_specifier(*format, ap);
			if (chars_written < 0)
				return (chars_written);
		}
		else
		{
			chars_written += write(1, format, 1);
			if (chars_written < 0)
				return (chars_written);
		}
		format++;
	}
	va_end(ap);
	return (chars_written);
}
