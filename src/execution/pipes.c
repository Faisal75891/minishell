/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 21:46:30 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/22 10:11:11 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: pipe implementation
// - parse pipe operators |
int	has_pipe(char *command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		if (command[i] == '|')
			return (1);
		i++;
	}
	return (0);
}

char	**split_pipeline(char *command)
{
	char	**commands;
	char	*trimmed_command;
	int		i;

	commands = ft_split(command, '|');
	if (!commands)
		return (NULL);
	i = 0;
	while (commands[i])
	{
		trimmed_command = ft_strtrim(commands[i], " \t\n\v\f\r");
		if (!trimmed_command)
		{
			free_split(commands);
			return (0);
		}
		free(commands[i]);
		commands[i] = trimmed_command;
		i++;
	}
	return (commands);
}

static void	exec_child(char *command, t_shell *shell)
{
	char	**arg_list;

	arg_list = get_args(command, shell->env);
	if (!arg_list)
		exit (1);
	execve(arg_list[0], arg_list, shell->env);
	if (errno == ENOENT)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg_list[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(127);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg_list[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	free_split(arg_list);
	exit(1);
}


void	pipeline(char *command, t_shell *shell, int prev_pipe, int pipe_fd[2], int index, int n)
{
	// setup I/O
	if (prev_pipe != -1)
		dup_and_close(prev_pipe, STDIN_FILENO);
	if (index < n - 1)
		dup_and_close(pipe_fd[1], STDOUT_FILENO);
	close_if_open(&pipe_fd[0]);
	// then execute the command
	exec_child(command, shell);
}

int	wait_all(int *pids, int n)
{
	int	i;
	int	status;
	int	last_status;

	last_status = 0;
	status = 0;
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (i == n - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}

// - handle multiple pipes
int	run_pipeline(char	*command, t_shell *shell)
{
	pid_t	pid;
	char	**cmds;
	int		pipe_fd[2];
	int		*pids;
	int		last_status;
	int		prev_pipe;
	int		n;
	int		i;

	cmds = split_pipeline(command);
	if (!cmds)
	{
		shell->last_status = 1;
		return (0);
	}
	prev_pipe = -1;
	n = split_len(cmds);
	pids = malloc(n * sizeof(n));
	if (!pids)
	{
		free_split(cmds);
		return (1);
	}
	i = 0;
	while (i < n)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		if (i < n - 1 && pipe(pipe_fd) == -1)
		{
			close_if_open(&prev_pipe);
			free_split(cmds);
			free(pids);
			return (1);
		}
		pid = fork();
		if (pid == 0)
			pipeline(cmds[i], shell, prev_pipe, pipe_fd, i, n);
		else if (pid < 0)
		{
			close_if_open(&prev_pipe);
			close_if_open(&pipe_fd[0]);
			close_if_open(&pipe_fd[1]);
			free_split(cmds);
			int j = 0;
			while (j < i)
			{
				waitpid(pids[j], NULL, 0);
				j++;
			}
			free(pids);
			return (1);
		}
		pids[i] = pid;
		close_if_open(&prev_pipe);
		if (i < n - 1)
		{
			close_if_open(&pipe_fd[1]);
			prev_pipe = pipe_fd[0];
		}
		i++;
	}
	last_status = wait_all(pids, n);
	close_if_open(&prev_pipe);
	free_split(cmds);
	free(pids);
	return (last_status);
}
