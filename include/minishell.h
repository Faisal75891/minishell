/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/15 02:30:03 by fbaras           ###   ########.fr       */
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
# include <errno.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

// stores the shell
// and the status of the last command.
typedef struct s_shell
{
    char    **env;
    int     last_status;
}   t_shell;

// cleanup.c
void	free_split(char **arr);

// path_utils.c
char	**get_paths(char **environ);
char	*get_full_path(char *command, char **paths);

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
int		handle_command(char *command, t_shell *shell);
int 	execute_command(char **arg_list, t_shell *shell);

// pipes.c - TODO
// redirects.c - TODO


// cd.c - TODO
// echo.c - TODO
// env.c - TODO
// export.c - TODO
// pwd.c - TODO
// unset.c - TODO
// exit.c - TODO

// init.c
char	**copy_env(char **envp);

// signals.c - TODO

#endif
