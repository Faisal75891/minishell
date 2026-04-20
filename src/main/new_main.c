/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:02:47 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/17 14:02:47 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
todo-list:

TODO: exit needs return exit number, it shouldn't take non numeric args
	e.g from bash: 
	$ exit 1 a;lkfads
		logout
		-bash: exit: too many arguments.

* TODO: Use the builtin functions instead of bash's              (priority)
	Make sure echo works correctly.
	export
	unset
	env

TODO: command not found doesn't show anymore for some reason.

TODO: $HOME, $USER or $PATH works but $? doesn't.

TODO: doing ctrl-c after using cat displays and extra "$".

TODO: ctrl-/ clears the prompt line after typing stuff Should do nothing instead.

TODO: prompting: ' "" ' should do nothing (i think).

TODO: cd doesn't work.

TODO: redirects should create files with 0644 permissions. (need to double check)


*/

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc (sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->last_status = 0;
	return (shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_lex_result	*lexer;
	t_parsed_result	*p;
	t_shell			*shell;
	char			*input;

	shell = init_shell(envp);
	if (!shell)
		return (1);
	lexer = init_lexer();
	if (!lexer)
	{
		free_split(shell->env);
		free(shell);
		return (1);
	}
	(void) argc;
	(void) argv;
	signal(SIGINT, handle_ctrl_c);
	//signal(SIGUSR1, handle_ctrl_slash);
	while (1)
	{
		input = readline("$ ");
		if (!input)
			break ;
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		if (ft_strncmp(input, "exit", 4) == 0)
		{
			free(input);
			break ;
		}
		tokenize_lexer(input, lexer);
		p = parser(lexer, shell);
		execute_commands(p, shell);
		add_history(input);
		clear_lexer(lexer);
		free_parser(p);
		free(input);
	}
	free_split(shell->env);
	free(shell);
	free(lexer);
	clear_history();
	return (0);
}
