/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/30 14:34:35 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>

// stores the shell
// and the status of the last command.
typedef struct s_shell
{
	char	**env;
	int		last_status;
}	t_shell;

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_HEREDOC
}	t_token_type;

typedef enum e_quote_type
{
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE
}	t_quote_type;

typedef struct s_token
{
	struct s_token			*next;
	t_token_type			type;
	t_quote_type			quote;
	char					*word;
}	t_token;

typedef struct s_redirections
{
	char			*target;
	t_token_type	type;
	t_quote_type	quote;
}	t_redirections;

typedef struct s_commands
{
	char				**argv;
	int					argc;
	t_redirections		*redirections;
	int					redirections_count;
}	t_commands;

typedef struct s_parsed_result
{
	t_commands	*commands;
	int			command_count;
	int			command_error;
}	t_parsed_result;

typedef struct s_lex_result
{
	t_token	*head;
	t_token	*tail;
	int		count;
	int		error;
	char	*unexpected_token;
	//char	*error_msg;
}	t_lex_result;

// pipe_utils
void	close_if_open(int *fd);
void	dup_and_close(int fd1, int fd2);
int	init_pipe_fd(int pipe_fd[2], int i, int count);
void	manage_pipe_fds(int *prev_pipe, int pipe_fd[2], int i, int count);
int	spawn_child_process(t_commands *command, t_shell *shell, int pipe_fd[2], int *prev_pipe);

// cleanup.c
void	free_split(char **arr);
int		split_len(char **arr);
void	free_parser(t_parsed_result *parser);


// path_utils.c
char	**get_paths(char **environ);
char	*get_full_path(char *command, char **paths);

// env_utils.c
char	*get_env_value(const char *name, char **env);

// parser.c
t_parsed_result	*parser(t_lex_result *lexer, t_shell *shell);

// old parser.
char			*get_full_command(char *command, char **environ);
char			**get_args(char *command, t_shell *shell);

// lexer.c
// HELLO. IF YOU HAVE BETTER NAMES YOU CAN CHANGE
t_lex_result	*init_lexer(void);
t_token			*create_token(t_token_type type, t_quote_type quote, const char *word);
void			append_token(t_lex_result *lexer, t_token *token);
void			tokenize_lexer(const char *command, t_lex_result *lexer);
void			clear_lexer(t_lex_result *lexer);
// lexer_utils.c
void	add_here_doc(t_lex_result *lexer);
void	add_append(t_lex_result *lexer);
void	add_pipe(t_lex_result *lexer);
void	add_redirect_out(t_lex_result *lexer);
void	add_redirect_in(t_lex_result *lexer);


// quotes.c
int				add_word(const char *command, t_lex_result *lexer, int *i);
// expansion.c
char	*expand_variables(const char *s, t_quote_type quote, t_shell *shell);

// executor.c
int		handle_command(char *command, t_shell *shell);
int		execute_command(char **arg_list, t_shell *shell);

// NEW_EXECUTOR
int	execute_commands(t_parsed_result *parsed_result, t_shell *shell);

// pipes.c - TODO
int		has_pipe(char *command);
int		run_pipeline(char	*command, t_shell *shell);

// redirects.c - TODO
int		wait_all(int *pids, int n);
void	handle_redirects(t_commands *command, t_shell *shell);

// builtisignal(SIGINT, SIG_DFL);
int		ms_cd(t_shell *shell, char **args);
int		ms_echo(t_shell *shell, char **args);
int		ms_env(t_shell *shell, char **args);
int		ms_export(t_shell *shell, char **args);
int		ms_pwd(t_shell *shell, char **args);
int		ms_unset(t_shell *shell, char **args);
int		ms_exit(t_shell *shell, char **args);

// init.c
char	**copy_env(char **envp);

// signals.c - TODO
int		get_last_signal(void);
void	signal_handler(int sig);
void	set_last_signal(int sig);
void	set_new_termios(int on);
void	new_signal_handler(void);
void	reset_signal_handler(void);
void	ignore_signal(void);

// error_utils.c
char	*ms_strappend_free(char *s1, char *s2);
char	*ms_strappend_char(char *s, char c);
int		word_fail(t_lex_result *lexer, char *buffer, int err);
int		word_commit(t_lex_result *lexer, char *buffer, int);
void	exit_error(char *command);


int		ms_is_var_char(int c, int first);

int		ft_isspace(char c);
int		is_operator_char(char c);
int		is_redirect(t_token_type token);
int		ms_str_has_dollar(const char *s);

#endif
