/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 08:29:13 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/27 17:01:53 by fbaras           ###   ########.fr       */
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
			;
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

void	set_new_termios(int on)
{
	struct termios	t;

	if (tcgetattr(STDIN_FILENO, &t) == -1)
		return ;
	if (on)
		t.c_lflag |= ECHOCTL;
	else
		t.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void	new_signal_handler(void)
{
	struct sigaction	sa;
	struct sigaction	sa2;

	ft_memset(&sa, 0, sizeof(sa2));
	sa2.sa_handler = SIG_IGN;
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &signal_handler;
	sa.sa_flags =  SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa2, NULL);
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
