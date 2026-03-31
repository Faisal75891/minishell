/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 11:38:58 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/30 11:39:11 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// first != 0 -> first char of variable name, else subsequent char
int	ms_is_var_char(int c, int first)
{
	if (first)
		return (ft_isalpha((unsigned char)c) || c == '_');
	return (ft_isalnum((unsigned char)c) || c == '_');
}
