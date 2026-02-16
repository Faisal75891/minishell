/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:41:41 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/16 16:41:41 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include <fcntl.h>

void	free_split(char **arr);

// read_commands
char	**get_paths(char **environ);
char	*get_full_path(char *command, char **paths);
char	*get_full_command(char *command, char **environ);
char	*read_command(void);
char	**get_args(char *command, char **environ);

// history
void	add_to_history(char *command);


#endif