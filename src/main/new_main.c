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

int	execute_builtin(t_commands *command, t_shell *shell)
{
	char	**args;

	if (command
		&& command->argv
		&& command->argv[0]
		&& command->argv[0][0] != '\0')
		args = command->argv;
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

int	handle_empty_signal(char *input, t_shell *shell, int status)
{
	if (status == SIGINT)
	{
		shell->last_status = status + 128;
		set_last_signal(0);
	}
	else
	{
		set_last_signal(0);
		return (0);
	}
	if (input)
		free(input);
	return (1);
}

static int	execute_command_or_builtin(t_parsed_result *p, t_shell *shell)
{
	int	builtins;

	if (!p)
		return (2);
	builtins = execute_builtin(p->commands, shell);
	if (builtins != -1)
		return (builtins);
	return (execute_commands(p, shell));
}

static int	read_and_execute_command(t_lex_result *lexer, t_shell *shell)
{
	t_parsed_result		*p;
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
	p = parser(lexer, shell);
	shell->last_status = execute_command_or_builtin(p, shell);
	add_history(input);
	clear_lexer(lexer);
	free_parser(p);
	free(input);
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
