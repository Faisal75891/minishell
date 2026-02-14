/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 17:17:51 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/07 17:17:51 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "../libft.h"

int			ft_putnbr_base_fd(unsigned int nbr, const char *base, int fd);
int			ft_putptr_fd(void *ptr, int fd);
int			ft_putnbr_fdi(int n, int i, int fd);
int			ft_putchar_fdi(char c, int fd);
int			ft_putstr_fdi(const char *s, int fd);
int			ft_printf(const char *format, ...);
int			ft_putunbr_fd(unsigned int nbr, int fd);

#endif