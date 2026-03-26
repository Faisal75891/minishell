/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/26 12:53:57 by fbaras           ###   ########.fr       */
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

typedef struct s_lex_result
{
	t_token	*head;
	t_token	*tail;
	int		count;
	int		error;
	//char	*error_msg;
}	t_lex_result;

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

// lexer.c
// HELLO. IF YOU HAVE BETTER NAMES YOU CAN CHANGE
t_lex_result	*init_lexer(void);
t_token			*create_token(t_token_type type, t_quote_type quote, char *word);
void			append_token(t_lex_result *lexer, t_token *token);
void			tokenize_lexer(char *command, t_lex_result *lexer);
void			clear_lexer(t_lex_result *lexer);
// lexer_utils.c
void	add_here_doc(t_lex_result *lexer);
void	add_append(t_lex_result *lexer);
void	add_pipe(t_lex_result *lexer);
void	add_redirect_out(t_lex_result *lexer);
void	add_redirect_in(t_lex_result *lexer);


// quotes.c - TODO
int				add_word(char *command, t_lex_result *lexer, int *i);
// expansion.c
char	*expand_variables(const char *s, t_shell *shell);

// executor.c
int		handle_command(char *command, t_shell *shell);
int		execute_command(char **arg_list, t_shell *shell);

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
int		word_fail(t_lex_result *lexer, char *buffer, int err);
int		word_commit(t_lex_result *lexer, char *buffer);



int		ms_is_var_char(int c, int first);

// str_utils.c
int		ft_isspace(char c);
int		is_operator_char(char c);
int		ms_str_has_dollar(const char *s);

#endif
