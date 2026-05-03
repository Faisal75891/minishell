/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 00:35:00 by samamaev          #+#    #+#             */
/*   Updated: 2026/05/02 00:28:15 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_read_loop(int write_fd, char *delimiter,
	t_quote_type quote, t_shell *shell)
{
	char	*line;
	char	*content;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strlen(line) > 0 && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		if (ft_strncmp(delimiter, line, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (quote == Q_NONE)
			content = expand_variables(line, Q_NONE, shell);
		else
			content = ft_strdup(line);
		ft_putendl_fd(content, write_fd);
		free(line);
		free(content);
	}
}

static void	handle_heredoc(char *delimiter, t_quote_type quote, t_shell *shell)
{
	int	here_doc[2];

	printf("reached here\n");
	signal(SIGQUIT, SIG_IGN);
	set_new_termios(0);
	if (pipe(here_doc) == -1)
		exit(1);
	heredoc_read_loop(here_doc[1], delimiter, quote, shell);
	close(here_doc[1]);
	dup_and_close(here_doc[0], STDIN_FILENO);
	set_new_termios(1);
}

int	open_redirect_fd(t_redirections *redir)
{
	if (redir->type == TOK_REDIR_APPEND)
		return (open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (redir->type == TOK_REDIR_IN)
		return (open(redir->target, O_RDONLY));
	if (redir->type == TOK_REDIR_OUT)
		return (open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	exit(1);
}

void	apply_redirection(t_redirections *redir, t_shell *shell)
{
	int	fd;

	if (!redir->target)
		exit(1);
	if (redir->type == TOK_HEREDOC)
	{
		handle_heredoc(redir->target, redir->quote, shell);
		return ;
	}
	fd = open_redirect_fd(redir);
	if (fd < 0)
		exit_error(redir->target);
	if (redir->type == TOK_REDIR_IN)
		dup_and_close(fd, STDIN_FILENO);
	else
		dup_and_close(fd, STDOUT_FILENO);
}
