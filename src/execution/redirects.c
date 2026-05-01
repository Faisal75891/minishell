/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:43:51 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 00:28:15 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_wait_status(int status, int *last_status, int print, int sq)
{
	if (print)
		write(1, "\n", 1);
	else if (sq)
	{
		if (WCOREDUMP(status))
			write(1, "quit (core dumped)\n", 19);
		else
			write(1, "quit\n", 5);
	}
	if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
	else
		*last_status = 128 + WTERMSIG(status);
}

int	wait_all(int *pids, int n)
{
	int	i;
	int	status;
	int	last_status;
	int	print;
	int	any_sigquit;

	print = 0;
	any_sigquit = 0;
	last_status = 0;
	i = -1;
	while (++i < n)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			break ;
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			print = 1;
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			any_sigquit = 1;
		if (i == n - 1)
			handle_wait_status(status, &last_status, print, any_sigquit);
	}
	new_signal_handler();
	return (last_status);
}

void	handle_redirects(t_commands *command, t_shell *shell)
{
	int				i;

	if (!command->redirections)
		return ;
	i = -1;
	while (++i < command->redirections_count)
	{
		apply_redirection(&command->redirections[i], shell);
	}
}
