/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 21:46:30 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/01 00:00:00 by githelper        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**split_pipeline(char *command)
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

	arg_list = get_args(command, shell);
	if (!arg_list)
		exit(1);
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

static int	spawn_pipeline_processes(char **cmds, t_shell *shell,
	int *pids, int n)
{
	int	prev_pipe;
	int	pipe_fd[2];
	int	i;
	pid_t	pid;

	prev_pipe = -1;
	i = 0;
	while (i < n)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		if (i < n - 1 && pipe(pipe_fd) == -1)
		{
			close_if_open(&prev_pipe);
			return (1);
		}
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (prev_pipe != -1)
				dup_and_close(prev_pipe, STDIN_FILENO);
			if (i < n - 1)
				dup_and_close(pipe_fd[1], STDOUT_FILENO);
			close_if_open(&pipe_fd[0]);
			exec_child(cmds[i], shell);
		}
		else if (pid < 0)
		{
			while (--i >= 0)
				waitpid(pids[i], NULL, 0);
			close_if_open(&prev_pipe);
			close_if_open(&pipe_fd[0]);
			close_if_open(&pipe_fd[1]);
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
	close_if_open(&prev_pipe);
	return (0);
}

static int	collect_pipeline_status(int *pids, int n)
{
	int	status;

	status = 0;
	while (n-- > 0)
		waitpid(pids[n], &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (status);
}

int	run_pipeline(char *command, t_shell *shell)
{
	char	**cmds;
	int	*pids;
	int	n;
	int	status;

	cmds = split_pipeline(command);
	if (!cmds)
	{
		shell->last_status = 1;
		return (0);
	}
	n = split_len(cmds);
	pids = malloc(sizeof(int) * n);
	if (!pids)
	{
		free_split(cmds);
		return (1);
	}
	if (spawn_pipeline_processes(cmds, shell, pids, n))
	{
		free_split(cmds);
		free(pids);
		return (1);
	}
	status = collect_pipeline_status(pids, n);
	free_split(cmds);
	free(pids);
	return (status);
}
