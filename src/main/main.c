/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/17 14:02:37 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*command;
	int		code;

	(void)argc;
	(void)argv;
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (1);
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		write(2, "env copy failed\n", 16);
		return (1);
	}
	shell->last_status = 0;
	// signal(SIGINT, handle_ctrl_c);
	// signal(SIGQUIT, handle_ctrl_slash);
	while (1)
	{
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
		free(command);
	}
	code = shell->last_status;
	free_split(shell->env);
	free(shell);
	clear_history();
	return (code);
}
