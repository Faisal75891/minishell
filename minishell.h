/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:41:41 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/16 14:59:21 by samamaev         ###   ########.fr    */
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
char	**get_paths(char **environ);
char	*get_full_path(char *command, char **paths);

// read_commands
char	*get_full_command(char *command, char **environ);
char	*read_command(void);
char	**get_args(char *command, char **environ);
int		is_history_key(char *command);

// history
void	add_to_history(char *command);

// minishell
int		handle_command(char *command, char **environ);
void	execute_command(char **arg_list, char **environ);

#endif