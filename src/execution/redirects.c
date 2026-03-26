/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:43:51 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/26 10:38:06 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// TODO: redirection implementation
// - handle < (input redirection)
// - handle > (output redirection)
// - handle >> (append)
// - handle << (heredoc)

// "<" refers to stdin
// ">" refers to stdout

/*
step1 -> Redirecting Input
step2 -> Redirecting Output
step3 -> Appending Redirected Output
step4 -> Redirecting Standard Output and Standard Error
step5 -> Appending Standard Output and Standard Error
step6 -> Here Documents
*/

int has_redirect(void)
{
	return (0);	
}

// make cat's stdout output into file.txt
int	redirect_output(void)
{
	return (0);
}
