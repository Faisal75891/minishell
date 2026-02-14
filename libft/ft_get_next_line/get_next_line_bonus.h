/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:38:19 by fbaras            #+#    #+#             */
/*   Updated: 2025/08/19 12:38:19 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

size_t	gnl_strlen(const char *s1);
char	*gnl_strdup(const char *s1);
char	*gnl_strchr(const char *s, int c);
char	*gnl_strjoin(char *s1, char *s2);
size_t	gnl_strlcpy(char *dst, const char *src, size_t dstsize);
char	*read_line(char *stash, char *buffer, int fd);
char	*extract_line(char *stash);
char	*reset_stash(char *stash);
char	*get_next_line(int fd);

#endif
