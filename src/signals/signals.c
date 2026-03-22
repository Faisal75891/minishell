/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 08:29:13 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/15 08:51:20 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: signal handling
// - Ctrl+C (SIGINT) -> new prompt
void	handle_ctrl_c(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	
}

// - Ctrl+D (EOF) -> exit 
void	handle_ctrl_d(int pid)
{
	(void)pid;
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
