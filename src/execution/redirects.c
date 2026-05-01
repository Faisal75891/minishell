/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 14:43:51 by fbaras            #+#    #+#             */
/*   Updated: 2026/03/26 10:38:06 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
step1 -> Redirecting Input
step2 -> Redirecting Output
step3 -> Appending Redirected Output
step4 -> Redirecting Standard Output and Standard Error
step5 -> Appending Standard Output and Standard Error
step6 -> Here Documents
*/

static void	handle_wait_status(int status, int *last_status, int print, int sq)
{
    if (print)
        write(1, "\n", 1);
    else if (sq)
    {
        if (WCOREDUMP(status))
            write(1, "quit (core dumped)\n", 19);
        else
            write(1, "quit\n", 5);
    }
    if (WIFEXITED(status))
        *last_status = WEXITSTATUS(status);
    else
        *last_status = 128 + WTERMSIG(status);
}

int	wait_all(int *pids, int n)
{
    int	i;
    int	status;
    int	last_status;
    int	print;
    int	any_sigquit;

    print = 0;
    any_sigquit = 0;
    last_status = 0;
    i = -1;
    while (++i < n)
    {
        if (waitpid(pids[i], &status, 0) == -1)
            break ;
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
            print = 1;
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
            any_sigquit = 1;
        if (i == n - 1)
            handle_wait_status(status, &last_status, print, any_sigquit);
    }
    new_signal_handler();
    return (last_status);
}

static void	setup_heredoc_pipe(int here_doc[2])
{
    signal(SIGQUIT, SIG_IGN);
    set_new_termios(0);
    if (pipe(here_doc) == -1)
        exit(1);
}

static void	handle_heredoc(char *delimiter, t_quote_type quote, t_shell *shell)
{
    char	*line;
    char	*content;
    int		here_doc[2];

    setup_heredoc_pipe(here_doc);
    while (1)
    {
        write(STDOUT_FILENO, "> ", 2);
        line = get_next_line(0);
        if (!line)
            break ;
        if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
            line[ft_strlen(line) - 1] = '\0';
        if (ft_strncmp(delimiter, line, ft_strlen(delimiter) + 1) == 0)
            break ;
        if (quote == Q_NONE)
            content = expand_variables(line, Q_NONE, shell);
        else
            content = ft_strdup(line);
        write(here_doc[1], content, ft_strlen(content));
        write(here_doc[1], "\n", 1);
        free(line);
        free(content);
    }
    close(here_doc[1]);
    dup_and_close(here_doc[0], STDIN_FILENO);
    set_new_termios(1);
}

void	handle_redirects(t_commands *command, t_shell *shell)
{
    int		        fd;
    int		        i;
    t_redirections	*redir;

    if (!command->redirections)
        return ;
    i = -1;
    while (++i < command->redirections_count)
    {
        redir = &command->redirections[i];
        if (redir->type == TOK_HEREDOC && redir->target)
            handle_heredoc(redir->target, redir->quote, shell);
        else if (redir->type == TOK_REDIR_APPEND && redir->target)
            fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (redir->type == TOK_REDIR_IN && redir->target)
            fd = open(redir->target, O_RDONLY);
        else if (redir->type == TOK_REDIR_OUT && redir->target)
            fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else
            exit(1);
        if (redir->type != TOK_HEREDOC && fd < 0)
            exit_error(redir->target);
        if (redir->type != TOK_HEREDOC)
            dup_and_close(fd, (redir->type == TOK_REDIR_IN) ? 0 : 1);
    }
}
