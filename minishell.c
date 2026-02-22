/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:41:36 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/16 14:59:21 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

void	execute_command(char **arg_list, char **environ)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		execve(arg_list[0], arg_list, environ);
		printf("no such file or directory\n");
		exit(1);
	}
	else if (pid > 0)
		wait(NULL);
	else
		printf("fork failed\n");
}

int	handle_command(char *command, char **environ)
{
	char	**arg_list;

	if (ft_strncmp(command, "exit", 5) == 0)
		return (1);
	arg_list = get_args(command, environ);
	if (!arg_list)
		return (0);
	execute_command(arg_list, environ);
	free_split(arg_list);
	return (0);
}

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
