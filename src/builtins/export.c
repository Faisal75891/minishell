/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/25 00:00:00 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_id(char *arg)
{
	int i;

	if (!arg || !*arg || (!ft_isalpha(*arg) && *arg != '_'))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_exports(t_shell *shell)
{
	int i;

	i = 0;
	while (shell->env[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putendl_fd(shell->env[i], 1);
		i++;
	}
}

static void	add_or_update_env(t_shell *shell, char *arg, int len)
{
	int i;
	char    **new_env;

	i = -1;
	while (shell->env[++i])
	{
		if (!ft_strncmp(shell->env[i], arg, len) && 
			(shell->env[i][len] == '=' || shell->env[i][len] == '\0'))
		{
			free(shell->env[i]);
			shell->env[i] = ft_strdup(arg);
			return ;
		}
	}
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return ;
	i = -1;
	while (shell->env[++i])
		new_env[i] = shell->env[i];
	new_env[i] = ft_strdup(arg);
	new_env[i + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
}

static int	export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
    ft_putstr_fd(arg, 2);
    ft_putendl_fd("': not a valid identifier", 2);
    return (1);
}

int	ms_export(t_shell *shell, char **args)
{
    int	i;
    int	len;
    int	status;

    if (!args[1])
        return (print_exports(shell), 0);
    status = 0;
	i = 0;
	while (args[++i])
	{
		if (!is_valid_id(args[i]))
			status = export_error(args[i]);
		else
		{
			len = 0;
			while (args[i][len] && args[i][len] != '=')
				len++;
			add_or_update_env(shell, args[i], len);
		}
	}
	return (status);
}
