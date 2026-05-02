/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 23:24:22 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/03 00:47:39 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_signal;

void	signal_handler(int sig)
{
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
	{
		if (sig == SIGINT)
		{
			g_signal = SIGINT;
			handle_sigint();
		}
		else if (sig == SIGQUIT)
			;
		else
			g_signal = 0; // check later.
	}
}

void	ignore_signal(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	reset_signal_handler(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	get_last_signal(void)
{
	return ((int)g_signal);
}

void	set_last_signal(int sig)
{
	g_signal = sig;
}
