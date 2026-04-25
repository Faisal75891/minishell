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

TODO: exit needs return exit number, it shouldn't take non numeric args. DONE

TODO: Use the builtin functions instead of bash's. DONE

TODO: cd doesn't work. DONE

TODO: redirects should create files with 0644 permissions. (need to double check). DONE

TODO: command not found doesn't show anymore for some reason. DONE

TODO: prompting: ' "" ' gives exit 126 should give 127. DONE 

TODO: segfault when one quote is entered (single or double) !!!!. DONE

TODO: doing ctrl-c after using cat displays and extra "$". DONE

TODO: ctrl-c should set last status to 130

TODO: ctrl + \ should be handled. And needs to set last_status to 131

*/

static int	execute_builtin(t_parsed_result *parser, t_shell *shell)
{
	char	**args;
	
	if (parser && parser->commands
		&& parser->commands->argv[0]
		&& parser->commands->argv
		&& parser->commands->argv[0][0] != '\0')
		args = parser->commands->argv;
	else
		return (-1);
	if (!ft_strncmp(args[0], "cd", 3))
		return (ms_cd(shell, args));
	if (!ft_strncmp(args[0], "pwd", 4))
		return (ms_pwd(shell, args));
	if (!ft_strncmp(args[0], "echo", 5))
		return (ms_echo(shell, args));
	if (!ft_strncmp(args[0], "export", 7))
		return (ms_export(shell, args));
	if (!ft_strncmp(args[0], "unset", 6))
		return (ms_unset(shell, args));
	if (!ft_strncmp(args[0], "env", 4))
		return (ms_env(shell, args));
	if (!ft_strncmp(args[0], "exit", 5))
		return (ms_exit(shell, args));
	return (-1);
}

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
	struct sigaction	sa; // For handling signals with sigaction
	struct termios		t_old;
	struct termios		t; // For removing "^C" and "^\"
	t_lex_result		*lexer;
	t_parsed_result		*p;
	t_shell				*shell;
	char				*input;

	tcgetattr(STDIN_FILENO, &t_old);
	t = t_old;
	t.c_lflag &= ~ECHOCTL; // Doesn't echo ^C ^\ ^....
	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &signal_handler;
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
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
	while (1)
	{
		// printf("last signal %d\n", get_last_signal());
		input = readline("$ ");
		if (!input)
			break ;
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		set_last_signal(0);
		tokenize_lexer(input, lexer);
		p = parser(lexer, shell);
		tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
		if (execute_builtin(p, shell) != -1)
		{
			add_history(input);
			free_parser(p);
			clear_lexer(lexer);
			free(input);
			continue ;
		}
		shell->last_status = execute_commands(p, shell);
		tcsetattr(STDIN_FILENO, TCSANOW, &t);
		add_history(input);
		clear_lexer(lexer);
		free_parser(p);
		free(input);
		// printf("shell: %d\n", shell->last_status);
	}
	free_split(shell->env);
	free(shell);
	free(lexer);
	rl_clear_history();
	return (0);
}
