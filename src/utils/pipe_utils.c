/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/22 08:08:55 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/22 08:08:55 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_if_open(int *fd)
{
	if (*fd > -1)
	{
		if (close(*fd) == -1)
			perror("close");
		*fd = -1;
	}
}

void	dup_and_close(int fd1, int fd2)
{
	if (fd1 == -1)
	{
		ft_putstr_fd("minishell: dup_and_close: invalid fd\n", 2);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd1, fd2) == -1)
	{
		perror("dup2");
		close_if_open(&fd1);
		exit(EXIT_FAILURE);
	}
	close_if_open(&fd1);
}

int	init_pipe_fd(int pipe_fd[2], int i, int count)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (i < count - 1 && pipe(pipe_fd) == -1)
		return (1);
	return (0);
}

void	manage_pipe_fds(int *prev_pipe, int pipe_fd[2], int i, int count)
{
	close_if_open(prev_pipe);
	if (i < count - 1)
	{
		close_if_open(&pipe_fd[1]);
		*prev_pipe = pipe_fd[0];
	}
}
