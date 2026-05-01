/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 12:57:11 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/21 18:15:17 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_full_command(char *command, char **environ)
{
	char	**paths;
	char	*full_path;

	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	paths = get_paths(environ);
	if (!paths)
		return (NULL);
	full_path = get_full_path(command, paths);
	free_split(paths);
	if (!full_path)
		return (ft_strdup(command));	
	return (full_path);
}

char	**get_args(char *command, t_shell *shell)
{
	int		i;
	char	**arg_list;
	char	*raw_cmd;
	char	*full_cmd;
	char	*expanded;

	arg_list = ft_split(command, ' ');
	if (!arg_list || !arg_list[0])
		return (arg_list);
	i = 0;
	while (arg_list[i])
	{
		expanded = expand_variables(arg_list[i], 0, shell);
		if (expanded)
		{
			free(arg_list[i]);
			arg_list[i] = expanded;
		}
		i++;
	}
	raw_cmd = arg_list[0];
	full_cmd = get_full_command(raw_cmd, shell->env);
	if (full_cmd)
	{
		free(raw_cmd);
		arg_list[0] = full_cmd;
	}
	return (arg_list);
}

// cat | grep "a" > file.txt
// LEXER: WORD(cat) PIPE(|) WORD(grep) WORD(a) REDIRECT(>) WORD(file.txt)
// ls -al
// LEXER: WORD(ls) WORD(-al)

// I will assume a command is a group of words together.

// The parser will seperate the input into a group of commands.

t_commands	*init_commands(t_parsed_result *parser)
{
	t_commands	*commands;
	int			i;

	commands = malloc(sizeof(t_commands) * parser->command_count);
	if (!commands)
		return (NULL);
	i = 0;
	while (i < parser->command_count)
	{
		commands[i].argv = NULL;
		commands[i].argc = 0;
		commands[i].redirections = NULL;
		commands[i].redirections_count = 0;
		i++;
	}
	return (commands);
}

t_parsed_result	*init_parser(t_lex_result *lexer)
{
	t_parsed_result	*parser;
	t_token			*current;

	parser = malloc (sizeof(t_parsed_result));
	if (!parser)
		return (NULL);
	parser->command_count = 1;
	parser->command_error = 0;
	current = lexer->head;
	while (current)
	{
		if (current->type == TOK_PIPE)
			parser->command_count++;
		current = current->next;
	}
	parser->commands = init_commands(parser);
	if (!parser->commands)
	{
		free(parser);
		return (NULL);
	}
	return (parser);
}

// Loop through the tokens until you find a | or NULL
// Fill the argv and argc of each command.
static t_redirections	*grow_redirections(t_commands *cmd,
	t_quote_type quote, t_token_type type, char *word)
{
	t_redirections	*new_arr;
	int				i;
	int				old_count;

	old_count = cmd->redirections_count;
	new_arr = malloc(sizeof(t_redirections) * (old_count + 1));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < old_count)
	{
		new_arr[i] = cmd->redirections[i];
		i++;
	}
	new_arr[i].target = word;
	new_arr[i].type = type;
	new_arr[i].quote = quote;
	return (new_arr);
}

static int	append_redir(t_commands *command, t_quote_type quote,
	t_token_type type, char *word)
{
	t_redirections	*copy;

	copy = grow_redirections(command, quote, type, word);
	if (!copy)
		return (0);
	free(command->redirections);
	command->redirections = copy;
	command->redirections_count++;
	return (1);
}

int	append_arg(t_commands *command, char *cmd)
{
	char	**new_argv;
	int		i;

	new_argv = malloc (sizeof(char *) * (command->argc + 2));
	if (!new_argv)
		return (0);
	i = 0;
	while (command->argv != NULL && i < command->argc)
	{
		new_argv[i] = command->argv[i];
		i++;
	}
	new_argv[i] = cmd;
	new_argv[i + 1] = NULL;
	free(command->argv);
	command->argv = new_argv;
	command->argc++;
	return (1);
}

int	handle_redirect(t_token **current, t_commands *command, t_shell *shell)
{
	t_token_type	type;
	char			*expanded_word;
	t_quote_type	quote;

	type = (*current)->type;
	*current = (*current)->next;
	if (!*current || (*current)->type != TOK_WORD)
	{
		shell->last_status = 2;
		return (0);
	}
	quote = (*current)->quote;
	if (type == TOK_HEREDOC)
		expanded_word = ft_strdup((*current)->word);
	else if (quote == Q_NONE)
		expanded_word = ft_strdup((*current)->word);
	else
		expanded_word = expand_variables((*current)->word, quote, shell);
	if (!expanded_word)
	{
		shell->last_status = 1;
		return (0);
	}
	if (!append_redir(command, quote, type, expanded_word))
	{
		shell->last_status = 1;
		if (expanded_word)
			free(expanded_word);
		return (0);
	}
	*current = (*current)->next;
	return (1);
}

int	parse_command(t_token **current, t_commands *command, t_shell *shell)
{
	char	*expanded_word;

	while (*current && (*current)->type != TOK_PIPE)
	{
		if ((*current)->type == TOK_WORD)
		{
			if ((*current)->quote == Q_SINGLE)
				expanded_word = ft_strdup((*current)->word);
			else
				expanded_word = expand_variables((*current)->word, (*current)->quote, shell);
			if (!expanded_word)
				return (shell->last_status = 1, 0);
			if (!append_arg(command, expanded_word))
			{
				free(expanded_word);
				return (shell->last_status = 1, 0);
			}
			*current = (*current)->next;
		}
		else if (is_redirect((*current)->type))
		{
			if (!handle_redirect(current, command, shell))
				return (0);
		}
		else
		{
			shell->last_status = 2;
			return (0);
		}
	}
	if (command->argc == 0 && command->redirections_count == 0)
	{
		shell->last_status = 2;
		return (0);
	}
	return (1);
}

int	parse_pipeline(t_token **current,
	t_parsed_result *parsed, t_shell *shell, int i)
{
	if (i >= parsed->command_count)
		return (0);
	if (!parse_command(current, &parsed->commands[i], shell))
		return (0);
	if (*current && (*current)->type == TOK_PIPE)
	{
		*current = (*current)->next;
		if (!*current)
		{
			shell->last_status = 2;
			return (0);
		}
		return (parse_pipeline(current, parsed, shell, i + 1));
	}
	return (1);
}

t_parsed_result	*parser(t_lex_result *lexer, t_shell *shell)
{
	t_parsed_result	*parsed;
	t_token			*current;

	if (!lexer || lexer->error > 0 || lexer->count <= 0 || lexer->head == NULL)
	{
		shell->last_status = 1;
		return (NULL);
	}
	parsed = init_parser(lexer);
	if (!parsed)
	{
		shell->last_status = 1;
		return (NULL);
	}
	current = lexer->head;
	if (!parse_pipeline(&current, parsed, shell, 0))
	{
		free_parser(parsed);
		return (NULL);
	}
	return (parsed);
}
