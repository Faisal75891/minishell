/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/03 01:58:07 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_args(char **arg_list, t_shell *shell)
{
	int			i;
	char		*expanded;

	i = 0;
	while (arg_list[i])
	{
		expanded = expand_variables(arg_list[i], Q_NONE, shell);
		if (expanded)
		{
			free(arg_list[i]);
			arg_list[i] = expanded;
		}
		i++;
	}
}

static void	resolve_command(char **arg_list, t_shell *shell)
{
	char	*raw_cmd;
	char	*full_cmd;

	raw_cmd = arg_list[0];
	full_cmd = get_full_command(raw_cmd, shell->env);
	if (full_cmd)
	{
		free(raw_cmd);
		arg_list[0] = full_cmd;
	}
}

char	*get_full_command(char *command, char **environ)
{
	char	**paths;
	char	*full_path;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	paths = get_paths(environ);
	full_path = get_full_path(command, paths);
	free_split(paths);
	if (!full_path)
		return (ft_strdup(command));
	return (full_path);
}

char	**get_args(char *command, t_shell *shell)
{
	char	**arg_list;

	arg_list = ft_split(command, ' ');
	if (!arg_list || !arg_list[0])
		return (arg_list);
	expand_args(arg_list, shell);
	resolve_command(arg_list, shell);
	return (arg_list);
}
