/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/25 00:00:00 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

int	main(void)
{
	char	*command;

	while (1)
	{
		write(1, "$ ", 2);
		command = read_command();
		if (!command)
			continue ;
		if (handle_command(command, environ))
		{
			free(command);
			break ;
		}
		free(command);
	}
	return (0);
}
