/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/25 13:57:20 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtin(char **args, t_shell *shell)
{
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

static void	handle_child_process(char **arg_list, t_shell *shell)
{
	char*cmd_path;
	char**paths;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	cmd_path = NULL;
	if (arg_list[0][0] == '/' || arg_list[0][0] == '.')
		cmd_path = ft_strdup(arg_list[0]);
	else if (shell->env)
	{
		paths = get_paths(shell->env);
		if (paths)
		{
			cmd_path = get_full_path(arg_list[0], paths);
			free_split(paths);
		}
	}
	if (!cmd_path)
		cmd_path = arg_list[0];
	execve(cmd_path, arg_list, shell->env);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg_list[0], 2);
	ft_putendl_fd(": command not found", 2);
	_exit(127);
}

static int	wait_for_child(pid_t pid)
{
	int status;
	int exit_status;

	exit_status = 1;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_status = 128 + WTERMSIG(status);
	return (exit_status);
}

int execute_command(char **arg_list, t_shell *shell)
{
	pid_t	pid;
	int exit_status;

	exit_status = execute_builtin(arg_list, shell);
	if (exit_status != -1)
		return (exit_status);
	pid = fork();
	if (pid == 0)
		handle_child_process(arg_list, shell);
	else if (pid > 0)
		exit_status = wait_for_child(pid);
	else
	{
		ft_putendl_fd("fork failed", 2);
		exit_status = 1;
	}
	return (exit_status);
}

int handle_command(char *command, t_shell *shell)
{
	char**arg_list;

	if (has_pipe(command))
		shell->last_status = run_pipeline(command, shell);
	else
	{
		arg_list = get_args(command, shell);
		if (!arg_list || !arg_list[0])
		{
			if (arg_list)
				free_split(arg_list);
			return (0);
		}
		shell->last_status = execute_command(arg_list, shell);
		if (!ft_strncmp(arg_list[0], "exit", 5) && shell->last_status != -1)
		{
			free_split(arg_list);
			return (1);
		}
		free_split(arg_list);
	}
	return (0);
}
