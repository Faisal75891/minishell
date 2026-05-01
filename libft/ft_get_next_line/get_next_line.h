/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:14:35 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/08 16:14:35 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

char	*get_next_line(int fd);
int		size_of_line(int fd);
char	*extract_line(char *temp);
char	*reset_temp(char *temp);
char	*gnl_strdup(const char *s);
char	*gnl_strjoin(char const *s1, char const *s2);
char	*gnl_strchr(char *s, int c);
size_t	gnl_strlen(const char *s);
char	*gnl_strdup(const char *s);
size_t	gnl_strlcpy(char *dst, const char *src, size_t dstsize);
char	*read_line(char *stash, char *buffer, int fd);

#endif