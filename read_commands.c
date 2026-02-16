/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:41:38 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/16 16:41:38 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char **environ)
{
	char	**paths;
	int		i;

	paths = NULL;
	i = 0;
	while (environ[i])
	{
		if (!ft_strncmp(environ[i], "PATH=", 5))
		{
			paths = ft_split(environ[i] + 5, ':');
			if (!paths)
				return (NULL);
			break ;
		}
		i++;
	}
	return (paths);
}

char	*get_full_path(char *command, char **paths)
{
	int		i;
	char	*temp_cmd;
	char	*full_path;

	if (!paths)
		return (NULL);
	i = 0;
	temp_cmd = ft_strjoin("/", command);
	if (!temp_cmd)
		return (NULL);
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], temp_cmd);
		if (access(full_path, X_OK) == 0)
		{
			free(temp_cmd);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free(temp_cmd);
	return (NULL);
}

// PATH is ":" seperated
char	*get_full_command(char *command,  char **environ)
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
	int		fd;

	fd = open(".history", O_RDONLY);
	command = get_next_line(0);
	if (!command)
		return (NULL);
	if (ft_strlen(command) <= 1)
	{
		free(command);
		return (NULL);
	}
	command[ft_strlen(command) - 1] = '\0';
	if (ft_strncmp(command, "\x1b[A", 3) == 0 || ft_strncmp(command, "\x1b[B", 3) == 0)
	{
		write(1, get_next_line(fd), 1000);
		return (NULL);
	}
	add_to_history(command);
	close(fd);
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
