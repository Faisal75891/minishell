/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/25 03:20:16 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: exit builtin
// - exit with no args -> exit with last exit status
// - exit <n> -> exit with status n
// - handle numeric argument errors

static int	is_numeric(char *str)
{
	int i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	handle_exit_error(char *arg, int type)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (type == 1)
	{
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": numeric argument required", 2);
	}
	else if (type == 2)
		ft_putendl_fd("too many arguments", 2);
}

int ms_exit(t_shell *shell, char **args)
{
	int status;

	ft_putendl_fd("exit", 1);
	if (!args[1])
		exit(shell->last_status);
	if (!is_numeric(args[1]))
	{
		handle_exit_error(args[1], 1);
		exit(255);
	}
	if (args[2])
	{
		handle_exit_error(NULL, 2);
		return (1);
	}
	status = ft_atoi(args[1]);
	exit(status % 256);
}
