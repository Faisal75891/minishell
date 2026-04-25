/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 08:29:13 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/25 15:39:04 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile	sig_atomic_t	g;

//  In interactive mode:
// ◦ ctrl-C displays a new prompt on a new line.
// ◦ ctrl-D exits the shell.
// ◦ ctrl-\ does nothing.

// Additional:
	// Don´t display ctrl-\ or ctrl-c

static void	handle_sigint(void)
{
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_handler(int sig)
{
	// if interactive
	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
	{
		if (sig == SIGINT)
		{
			g = SIGINT;
			handle_sigint();
		}
		else if (sig == SIGQUIT)
		{
			// Don't save this signal
			rl_on_new_line();
			rl_redisplay();
		}
		else
			g = 0;
	}
}

int	get_last_signal(void)
{
	return ((int)g);
}

void	set_last_signal(int sig)
{
	g = sig;
}