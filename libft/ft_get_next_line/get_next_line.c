/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:17:44 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/08 16:17:44 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*extract_line(char *stash)
{
	int		i;
	char	*new_line;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	new_line = malloc(i + 1);
	if (!new_line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	gnl_strlcpy(new_line, stash, i + 1);
	return (new_line);
}

char	*reset_stash(char *stash)
{
	char	*new_stash;
	char	*nl_position;

	new_stash = NULL;
	nl_position = gnl_strchr(stash, '\n');
	if (!nl_position)
	{
		free(stash);
		return (NULL);
	}
	if (*(nl_position + 1))
	{
		new_stash = gnl_strdup(nl_position + 1);
		if (!new_stash)
		{
			free(stash);
			return (NULL);
		}
	}
	free(stash);
	return (new_stash);
}

size_t	gnl_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize == 0)
		return (gnl_strlen(src));
	while (src[i] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (gnl_strlen(src));
}

char	*read_line(char *stash, char *buffer, int fd)
{
	char	*temp;
	int		bytes;

	bytes = 1;
	while ((bytes > 0) && (!stash || !gnl_strchr(stash, '\n')))
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(stash);
			return (NULL);
		}
		if (bytes == 0)
			break ;
		buffer[bytes] = '\0';
		temp = gnl_strjoin(stash, buffer);
		free(stash);
		stash = temp;
		if (!stash)
			return (NULL);
	}
	if (bytes < 0 && (!stash || stash[0] == '\0'))
		return (free(stash), stash = NULL, NULL);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*new_line;
	char		buffer[BUFFER_SIZE + 1];

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (free(stash), stash = NULL, NULL);
	stash = read_line(stash, buffer, fd);
	new_line = extract_line(stash);
	stash = reset_stash(stash);
	return (new_line);
}
