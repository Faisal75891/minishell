/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 19:43:36 by fbaras            #+#    #+#             */
/*   Updated: 2025/07/17 19:43:36 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

static int	int_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*string;
	int		len;
	long	nbr;

	nbr = n;
	len = int_len(nbr);
	string = malloc(len + 1);
	if (!string)
		return (NULL);
	string[len] = '\0';
	if (nbr < 0)
	{
		string[0] = '-';
		nbr = -nbr;
	}
	if (nbr == 0)
		string[0] = '0';
	while (nbr > 0)
	{
		string[--len] = (nbr % 10) + '0';
		nbr /= 10;
	}
	return (string);
}
