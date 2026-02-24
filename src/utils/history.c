/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:41:30 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/22 15:28:52 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// will add the command entered to the .history file.
void	add_to_history(char *command)
{
	int	fd;

	fd = open(".history", O_RDWR | O_APPEND);
	if (fd < 0)
	{
		printf("couldn't add to history\n");
		return ;
	}
	ft_putendl_fd(command, fd);
	close(fd);
}
