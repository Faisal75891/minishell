/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/25 00:00:00 by samamaev         ###   ########.fr       */
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
	temp_cmd = ft_strjoin("/", command);
	if (!temp_cmd)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = ft_strjoin(paths[i], temp_cmd);
		if (access(full_path, X_OK) == 0)
			return (free(temp_cmd), full_path);
		free(full_path);
	}
	return (free(temp_cmd), NULL);
}
