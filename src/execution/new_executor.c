/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 12:19:20 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/01 12:19:20 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_error(char *command)
{
	if (errno == ENOENT)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(127);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	exit(1);
}

static void	exec_child(t_commands *command, t_shell *shell)
{
	char	*full_command;

	if (command->argv[0][0] == '\0')
		full_command = strdup(command->argv[0]);
	else
	{
		full_command = get_full_command(command->argv[0], shell->env);
		if (!full_command)
			exit(1);
	}
	execve(full_command, command->argv, shell->env);
	free(full_command);
	exit_error(command->argv[0]);
}

static int	wait_all(int *pids, int n)
{
	int	i;
	int	status;
	int	last_status;

	last_status = 0;
	status = 0;
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (i == n - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}

// reads from stdin and writes heredoc contents into a pipe.
// when delimiter is unquoted, expand variables in each line.
// TODO: handle signals (chatgpt told me).
static void	handle_heredoc(char *delimiter, t_quote_type quote, t_shell *shell)
{
	char	*line;
	char	*content;
	int		here_doc[2];
	int		len;

	if (pipe(here_doc) == -1)
		exit(1);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(0);
		if (!line)
		{
			close(here_doc[0]);
			close(here_doc[1]);
			return ;
		}
		len = (int)ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strncmp(delimiter, line, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (quote == Q_NONE)
			content = expand_variables(line, Q_NONE, shell);
		else
			content = ft_strdup(line);
		free(line);
		if (!content)
		{
			close(here_doc[0]);
			close(here_doc[1]);
			exit(1);
		}
		write(here_doc[1], content, ft_strlen(content));
		write(here_doc[1], "\n", 1);
		free(content);
	}
	close(here_doc[1]);
	dup_and_close(here_doc[0], STDIN_FILENO);
}

static void	handle_redirects(t_commands	*command, t_shell *shell)
{
	int	fd;
	int	i;

	if (!command->redirections)
		return ;
	i = 0;
	while (i < command->redirections_count)
	{
		if (command->redirections[i].type == TOK_HEREDOC && command->redirections[i].target)
			handle_heredoc(command->redirections[i].target,
				command->redirections[i].quote, shell);
		else if (command->redirections[i].type == TOK_REDIR_APPEND && command->redirections[i].target)
		{
			fd = open(command->redirections[i].target, O_RDWR | O_CREAT | O_APPEND);
			if (fd < 0)
				exit_error(command->redirections[i].target);
			dup_and_close(fd, STDOUT_FILENO);
		}
		else if (command->redirections[i].type == TOK_REDIR_IN && command->redirections[i].target)
		{
			fd = open(command->redirections[i].target, O_RDONLY);
			if (fd < 0)
				exit_error(command->redirections[i].target);
			dup_and_close(fd, STDIN_FILENO);
		}
		else if (command->redirections[i].type == TOK_REDIR_OUT && command->redirections[i].target)
		{
			fd = open(command->redirections[i].target, O_RDWR | O_CREAT | O_TRUNC);
			if (fd < 0)
				exit_error(command->redirections[i].target);
			dup_and_close(fd, STDOUT_FILENO);
		}
		else
			exit(1);
		i++;
	}
}

void	execute_single_command(t_commands *command, t_shell *shell, int pipe_fd[2], int *prev_pipe)
{
	//  1. pipe
	if (*prev_pipe != -1)
		dup_and_close(*prev_pipe, STDIN_FILENO);
	if (pipe_fd[1] != -1)
		dup_and_close(pipe_fd[1], STDOUT_FILENO);
	close_if_open(&pipe_fd[0]);
	//  2. redirect
	handle_redirects(command, shell);
	// 	3. execute
	exec_child(command, shell);
}

int	execute_commands(t_parsed_result *parsed_result, t_shell *shell)
{
	pid_t	pid;
	int		*pids;
	int		prev_pipe;
	int		pipe_fd[2];
	int		last_status;
	int		i;

	i = 0;
	last_status = 0;
	prev_pipe = -1;
	if (!parsed_result)
	{
		return (0);
	}
	printf("reached here\n");
	pids = malloc (sizeof(int) * parsed_result->command_count);
	if (!pids)
		return (1);
	while (i < parsed_result->command_count)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		if (i < parsed_result->command_count - 1 && pipe(pipe_fd) == -1)
			return (1);
		pid = fork();
		if (pid == 0)
			execute_single_command(&parsed_result->commands[i], shell, pipe_fd, &prev_pipe);
		if (pid < 0)
		{
			close_if_open(&pipe_fd[0]);
			close_if_open(&pipe_fd[1]);
			free(pids);
			perror("fork");
			return (1);
		}
		else
		{
			pids[i] = pid;
			close_if_open(&prev_pipe);
			if (i < parsed_result->command_count - 1)
			{
				close_if_open(&pipe_fd[1]);
				prev_pipe = pipe_fd[0];
			}
		}
		i++;
	}
	last_status = wait_all(pids, parsed_result->command_count);
	close_if_open(&pipe_fd[0]);
	close_if_open(&pipe_fd[1]);
	free(pids);
	return (last_status);
}
