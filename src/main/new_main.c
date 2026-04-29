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
	//struct termios		t_old;
	t_lex_result		*lexer;
	t_parsed_result		*p;
	t_shell				*shell;
	char				*input;

	if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
		printf("Starting in interactive mode\n");
	else
		printf("non-interactive mode\n");
	//tcgetattr(STDIN_FILENO, &t_old);
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
		char	*ps1 = get_env_value("PWD", shell->env);
		new_signal_handler();
		set_new_termios(0);
		// write(1, ps1, ft_strlen(ps1));
		// write(1, "\n", 1);
		input = readline("$ ");
		if (!input)
			break ;
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		tokenize_lexer(input, lexer);
		p = parser(lexer, shell);
		int builtins = execute_builtin(p, shell); 
		if (builtins != -1)
		{
			shell->last_status = builtins;
			add_history(input);
			free_parser(p);
			clear_lexer(lexer);
			free(input);
			continue ;
		}
		else
			shell->last_status = execute_commands(p, shell);
		add_history(input);
		clear_lexer(lexer);
		free_parser(p);
		free(input);
	}
	//tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
	free_split(shell->env);
	free(shell);
	free(lexer);
	rl_clear_history();
	return (0);
}
