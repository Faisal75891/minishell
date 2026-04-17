/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 08:29:13 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/17 16:30:53 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// I think signals needs a global variable.

// TODO: signal handling
// - Ctrl+C (SIGINT) -> new prompt
void	handle_ctrl_c(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// - Ctrl+D (EOF) -> exit 
void	handle_ctrl_d(int pid)
{
	(void)pid;
	printf("hellooo\n");
	//kill(SIGINT, pid);
	// call cleanup
}

// - Ctrl+\ (SIGQUIT) -> do nothing in interactive mode
void	handle_ctrl_slash(int sig)
{
	(void)sig;
	return ;
}
// - different behavior in child processes
