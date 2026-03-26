/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/25 14:46:40 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_full_command(char *command, char **environ)
{
	char	**paths;
	char	*full_path;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	paths = get_paths(environ);
	if (!paths)
		return (NULL);
	full_path = get_full_path(command, paths);
	free_split(paths);
	return (full_path);
}

char	**get_args(char *command, t_shell *shell)
{
	char	**arg_list;
	char	*raw_cmd;
	char	*full_cmd;
	int		i;
	char	*expanded;

	arg_list = ft_split(command, ' ');
	if (!arg_list || !arg_list[0])
		return (arg_list);
	// variable expansion on each argument
	i = 0;
	while (arg_list[i])
	{
		expanded = expand_variables(arg_list[i], shell);
		if (expanded)
		{
			free(arg_list[i]);
			arg_list[i] = expanded;
		}
		i++; 
	}
	raw_cmd = arg_list[0];
	full_cmd = get_full_command(raw_cmd, shell->env);
	if (full_cmd)
	{
		free(raw_cmd);
		arg_list[0] = full_cmd;
	}
	return (arg_list);
}
