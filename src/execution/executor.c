/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/15 02:37:32 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(char **arg_list, t_shell *shell)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	exit_status = 1;
	pid = fork();
	if (pid == 0)
	{
		execve(arg_list[0], arg_list, shell->env);
		if (errno == ENOENT)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg_list[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			exit(127);
		}
		else if (errno == EACCES)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(arg_list[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
		else
			exit(1);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_status = 128 + WTERMSIG(status);
	}
	else
		ft_putendl_fd("fork failed", 2);
	return (exit_status);
}

int	handle_command(char *command, t_shell *shell)
{
	char	**arg_list;

	if (ft_strncmp(command, "exit", 5) == 0)
		return (1);
	arg_list = get_args(command, shell->env);
	if (!arg_list)
	{
		shell->last_status = 1;
		return (0);
	}
	shell->last_status = execute_command(arg_list, shell);
	free_split(arg_list);
	return (0);
}
