/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/23 01:05:00 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
#include <signal.h>
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

// will use later for norminette
// typedef struct s_piping
// {
// 	char	**argv;
// 	int		argc;
// 	int		index;
// 	t_shell	*shell;
// 	int		pipe[2];
// }	t_piping;

// pipe_utils
void	close_if_open(int *fd);
void	dup_and_close(int fd1, int fd2);

// cleanup.c
void	free_split(char **arr);
int		split_len(char **arr);

// path_utils.c
char	**get_paths(char **environ);
char	*get_full_path(char *command, char **paths);

// env_utils.c
char	*get_env_value(const char *name, char **env);

// parser.c
char	*get_full_command(char *command, char **environ);
char	*read_command(void);
char	**get_args(char *command, t_shell *shell);

// lexer.c - TODO
// quotes.c - TODO
// expansion.c
char	*expand_variables(const char *s, t_shell *shell);

// executor.c
int		handle_command(char *command, t_shell *shell);
int 	execute_command(char **arg_list, t_shell *shell);

// pipes.c - TODO
int		has_pipe(char *command);
int		run_pipeline(char	*command, t_shell *shell);

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
void	handle_ctrl_c(int sig);
void	handle_ctrl_slash(int sig);

// error_utils.c
char	*ms_strappend_free(char *s1, char *s2);
char	*ms_strappend_char(char *s, char c);

int	ms_is_var_char(int c, int first);
int	ms_str_has_dollar(const char *s);

#endif
