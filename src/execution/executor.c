/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:19:20 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/30 12:19:20 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_commands *command, t_shell *shell)
{
	char	*full_command;

	if (command->argv[0][0] == '\0')
	{
		full_command = ft_strdup(command->argv[0]);
		if (!full_command)
		{
			perror("malloc");
			exit(1);
		}
	}
	else
	{
		full_command = get_full_command(command->argv[0], shell->env);
		if (!full_command)
			execve(command->argv[0], command->argv, shell->env);
	}
	execve(full_command, command->argv, shell->env);
	free(full_command);
	exit_error(command->argv[0]);
}

void	execute_single_command(t_commands *cmd, t_shell *shell,
	int pipe_fd[2], int *prev_pipe)
{
	if (*prev_pipe != -1)
		dup_and_close(*prev_pipe, STDIN_FILENO);
	if (pipe_fd[1] != -1)
		dup_and_close(pipe_fd[1], STDOUT_FILENO);
	close_if_open(&pipe_fd[0]);
	handle_redirects(cmd, shell);
	exec_child(cmd, shell);
}

static int	spawn_child_process(t_commands *command, t_shell *shell,
	int pipe_fd[2], int *prev_pipe)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		set_new_termios(1);
		execute_single_command(command, shell, pipe_fd, prev_pipe);
	}
	return (pid);
}

static int	finish_exec(int *pids, int count, int p_fd[2])
{
	int	status;

	ignore_signal();
	status = wait_all(pids, count);
	close_if_open(&p_fd[0]);
	close_if_open(&p_fd[1]);
	free(pids);
	return (status);
}

int	execute_commands(t_parsed_result *parsed, t_shell *shell)
{
	int	*pids;
	int	prev_p;
	int	p_fd[2];
	int	i;

	if (!parsed)
		return (1);
	pids = malloc(sizeof(int) * parsed->command_count);
	if (!pids)
		return (1);
	i = -1;
	prev_p = -1;
	while (++i < parsed->command_count)
	{
		if (init_pipe_fd(p_fd, i, parsed->command_count))
			return (1);
		pids[i] = spawn_child_process(&parsed->commands[i],
				shell, p_fd, &prev_p);
		if (pids[i] < 0)
			return (1);
		manage_pipe_fds(&prev_p, p_fd, i, parsed->command_count);
	}
	return (finish_exec(pids, parsed->command_count, p_fd));
}
