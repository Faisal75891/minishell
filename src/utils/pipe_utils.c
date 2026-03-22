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
		ft_putstr_fd("pipex: dup_and_close: invalid fd\n", 2);
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
