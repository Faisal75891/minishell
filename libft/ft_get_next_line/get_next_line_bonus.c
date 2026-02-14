/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:26:08 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/19 12:26:08 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		buffer[BUFFER_SIZE + 1];
	char		*line;

	if ((fd > 1024 || fd < 0) || BUFFER_SIZE <= 0)
		return (free(stash[fd]), stash[fd] = NULL, NULL);
	stash[fd] = read_line(stash[fd], buffer, fd);
	line = extract_line(stash[fd]);
	stash[fd] = reset_stash(stash[fd]);
	return (line);
}

char	*read_line(char *stash, char *buffer, int fd)
{
	int		bytes_read;
	char	*temp;

	bytes_read = 1;
	while ((!stash || !gnl_strchr(stash, '\n')) && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			if (stash)
				free(stash);
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		temp = gnl_strjoin(stash, buffer);
		free(stash);
		stash = temp;
		if (!stash)
			return (NULL);
	}
	if (bytes_read <= 0 && (!stash || stash[0] == '\0'))
		return (free(stash), stash = NULL, NULL);
	return (stash);
}

char	*extract_line(char *stash)
{
	char	*line;
	int		i;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	gnl_strlcpy(line, stash, i + 1);
	return (line);
}

char	*reset_stash(char *stash)
{
	char	*new_stash;
	char	*nl_position;

	new_stash = NULL;
	if (!stash)
		return (NULL);
	nl_position = gnl_strchr(stash, '\n');
	if (!nl_position)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	if (*(nl_position + 1))
	{
		new_stash = gnl_strdup(nl_position + 1);
		if (!new_stash)
		{
			free(stash);
			stash = NULL;
			return (NULL);
		}
	}
	free(stash);
	return (new_stash);
}
