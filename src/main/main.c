/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/13 22:32:10 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

int	main(void)
{
	char	*command;

	while (1)
	{
		// TODO: handle ctrl + c signal and make it 
		command = readline("$ ");
		if (!command)
			break ;
		if (*command == '\0')
		{
			free(command);
			continue ;
		}
		add_history(command); // TODO: handle consequetive same commands. Don't keep duplicate
		if (handle_command(command, environ))
		{
			free(command);
			break ;
		}
		free(command);
	}
	return (0);
}
