/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/25 00:00:00 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: unset builtin
// - unset VAR -> remove from environment
// - handle multiple variables

static void	remove_env_var(t_shell *shell, char *var)
{
	int i;
	int j;
	size_t	len;

	len = ft_strlen(var);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var, len) == 0 && shell->env[i][len] == '=')
		{
			free(shell->env[i]);
			j = i;
			while (shell->env[j])
			{
				shell->env[j] = shell->env[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

int ms_unset(t_shell *shell, char **args)
{
	int i;

	if (!args || !args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		remove_env_var(shell, args[i]);
		i++;
	}
	return (0);
}
