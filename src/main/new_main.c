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

	if (parser != NULL && parser->commands
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

static int	read_and_execute_command(t_lex_result *lexer, t_shell *shell)
{
	t_parsed_result		*p;
	char				*input;
	int					builtins;

	input = readline("$ ");
	if (!input || input[0] == '\0')
	{
		if (input)
			return (free(input), 1);
		return (0);
	}
	tokenize_lexer(input, lexer);
	p = parser(lexer, shell);
	builtins = execute_builtin(p, shell);
	if (builtins != -1)
		shell->last_status = builtins;
	else
		shell->last_status = execute_commands(p, shell);
	add_history(input);
	clear_lexer(lexer);
	free_parser(p);
	free(input);
	return (1);
}

static int	init_lexer_and_shell(t_lex_result **lexer,
	t_shell **shell, char **envp)
{
	*shell = init_shell(envp);
	if (!(*shell))
		return (0);
	*lexer = init_lexer();
	if (!(*lexer))
	{
		free_split((*shell)->env);
		free(*shell);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	struct termios		t_old;
	t_lex_result		*lexer;
	t_shell				*shell;

	tcgetattr(STDIN_FILENO, &t_old);
	if (!init_lexer_and_shell(&lexer, &shell, envp))
		return (1);
	(void) argc;
	(void) argv;
	while (1)
	{
		new_signal_handler();
		set_new_termios(0);
		if (!read_and_execute_command(lexer, shell))
			break ;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
	free_split(shell->env);
	free(shell);
	free(lexer);
	clear_history();
	return (0);
}
