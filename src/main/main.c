/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/22 09:45:57 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*command;

	(void)argc;
	(void)argv;
	shell = malloc (sizeof(t_shell));
	shell->env = copy_env(envp);
	while (1)
	{
		// TODO: handle ctrl + c signal and make it add a new line
		command = readline("$ ");
		if (!command)
			break ;
		if (*command == '\0')
		{
			free(command);
			continue ;
		}
		if (handle_command(command, shell))
		{
			free(command);
			break ;
		}
		add_history(command);
		printf("last exit status: %d\n", shell->last_status);
		free(command);
	}
	free_split(shell->env);
	free(shell);
	rl_clear_history();
	return (0);
}
