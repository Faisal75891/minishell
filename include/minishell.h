/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/25 00:00:00 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libft/libft.h"


// cleanup.c
void	free_split(char **arr);

// path_utils.c
char	**get_paths(char **environ);
char	*get_full_path(char *command, char **paths);

// history.c
void	add_to_history(char *command);

// env_utils.c - TODO
// error_utils.c - TODO


// parser.c
char	*get_full_command(char *command, char **environ);
char	*read_command(void);
char	**get_args(char *command, char **environ);
int		is_history_key(char *command);

// lexer.c - TODO
// quotes.c - TODO
// expansion.c - TODO


// executor.c
int		handle_command(char *command, char **environ);
void	execute_command(char **arg_list, char **environ);

// pipes.c - TODO
// redirects.c - TODO


// cd.c - TODO
// echo.c - TODO
// env.c - TODO
// export.c - TODO
// pwd.c - TODO
// unset.c - TODO
// exit.c - TODO


// signals.c - TODO

#endif
