/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/13 22:32:57 by fbaras           ###   ########.fr       */
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

char	*read_command(void)
{
	char	*command;

	command = get_next_line(0);
	if (!command)
		return (NULL);
	if (ft_strlen(command) <= 1)
	{
		free(command);
		return (NULL);
	}
	command[ft_strlen(command) - 1] = '\0';
	add_to_history(command);
	return (command);
}

char	**get_args(char *command, char **environ)
{
	char	**arg_list;
	char	*raw_cmd;
	char	*full_cmd;

	arg_list = ft_split(command, ' ');
	if (!arg_list || !arg_list[0])
		return (arg_list);
	raw_cmd = arg_list[0];
	full_cmd = get_full_command(raw_cmd, environ);
	if (full_cmd)
	{
		free(raw_cmd);
		arg_list[0] = full_cmd;
	}
	return (arg_list);
}
