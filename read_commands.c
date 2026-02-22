/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:41:38 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/16 14:59:21 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// PATH is ":" seperated
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

int	is_history_key(char *command)
{
	char	*history_line;
	int		fd;

	if (ft_strncmp(command, "\x1b[A", 3) != 0
		&& ft_strncmp(command, "\x1b[B", 3) != 0)
		return (0);
	fd = open(".history", O_RDONLY);
	if (fd >= 0)
	{
		history_line = get_next_line(fd);
		if (history_line)
			free(history_line);
		close(fd);
	}
	return (1);
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
	if (is_history_key(command))
	{
		free(command);
		return (NULL);
	}
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
