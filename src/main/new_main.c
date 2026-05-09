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

int	execute_builtin(t_shell *shell)
{
	char	**args;

	if (shell && shell->parser && shell->parser->commands
		&& shell->parser->commands->argv
		&& shell->parser->commands->argv[0]
		&& shell->parser->commands->argv[0][0] != '\0')
		args = shell->parser->commands->argv;
	else
		return (-1);
	if (!ft_strncmp(args[0], "cd", 3))
		return (ms_cd(shell, args));
	if (!ft_strncmp(args[0], "pwd", 4))
		return (ms_pwd(shell, args));
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

static int	execute_command_or_builtin(t_shell *shell)
{
	int	builtins;

	if (!shell || !shell->parser)
		return (2);
	builtins = execute_builtin(shell);
	if (builtins != -1)
		return (builtins);
	return (execute_commands(shell));
}

static int	read_and_execute_command(t_lex_result *lexer, t_shell *shell)
{
	char				*input;
	int					status;

	input = readline("$ ");
	status = get_last_signal();
	if (handle_empty_signal(input, shell, status))
		return (1);
	if (!input || input[0] == '\0')
	{
		if (input)
			return (free(input), 1);
		return (0);
	}
	tokenize_lexer(input, lexer);
	shell->parser = parser(lexer, shell);
	shell->last_status = execute_command_or_builtin(shell);
	add_history(input);
	clear_lexer(lexer);
	free_parser(shell->parser);
	shell->parser = NULL;
	free(input);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_lex_result		*lexer;
	t_shell				*shell;

	if (!init_lexer_and_shell(&lexer, &shell, envp))
		return (1);
	tcgetattr(STDIN_FILENO, &shell->t_old);
	(void) argc;
	(void) argv;
	while (1)
	{
		new_signal_handler();
		set_new_termios(0);
		if (!read_and_execute_command(lexer, shell))
			break ;
	}
	cleanup_shell(shell);
	return (0);
}
