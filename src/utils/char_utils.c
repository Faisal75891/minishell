/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>              +#+  +:+       +#+    */
/*                                                +#+#+#+#+#+   +#+           */
/*    									               #+#    #+#             */
/*   Created: 2026/03/23 00:45:00 by samamaev         ###   ########.fr       */
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
