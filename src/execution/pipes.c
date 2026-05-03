/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 21:46:30 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 00:12:18 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ms_split_pipeline(char *command);

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
	if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(arg_list[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	free_split(arg_list);
	exit(1);
}

static void	exec_pipeline_child(int prev_pipe, int pipe_fd[2],
	char *cmd, t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_pipe != -1)
		dup_and_close(prev_pipe, STDIN_FILENO);
	if (pipe_fd[1] != -1)
		dup_and_close(pipe_fd[1], STDOUT_FILENO);
	close_if_open(&pipe_fd[0]);
	exec_child(cmd, shell);
}

static int	spawn_error(int *pids, int i, int *prev_pipe, int pipe_fd[2])
{
	close_if_open(prev_pipe);
	close_if_open(&pipe_fd[0]);
	close_if_open(&pipe_fd[1]);
	wait_all(pids, i);
	return (1);
}

static int	spawn_pipeline_processes(char **cmds, t_shell *shell,
	int *pids, int n)
{
	int		prev_pipe;
	int		pipe_fd[2];
	int		i;
	pid_t	pid;

	prev_pipe = -1;
	i = -1;
	while (++i < n)
	{
		if (init_pipe_fd(pipe_fd, i, n))
			return (spawn_error(pids, i, &prev_pipe, pipe_fd));
		pid = fork();
		if (pid == 0)
			exec_pipeline_child(prev_pipe, pipe_fd, cmds[i], shell);
		if (pid < 0)
			return (spawn_error(pids, i, &prev_pipe, pipe_fd));
		pids[i] = pid;
		manage_pipe_fds(&prev_pipe, pipe_fd, i, n);
	}
	return (0);
}

int	run_pipeline(char *command, t_shell *shell)
{
	char	**cmds;
	int		*pids;
	int		n;
	int		status;

	cmds = ms_split_pipeline(command);
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
		status = 1;
	else
		status = wait_all(pids, n);
	free_split(cmds);
	free(pids);
	return (status);
}
