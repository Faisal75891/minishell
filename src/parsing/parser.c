/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 12:57:11 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/06 19:58:42 by fbaras           ###   ########.fr       */
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
		expanded = expand_variables(arg_list[i], shell);
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
		commands[i].heredoc = -1;
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
	t_token_type type, char *word)
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
	new_arr[i].target = ft_strdup(word);
	if (!new_arr[i].target)
		return (free(new_arr), NULL);
	new_arr[i].type = type;
	return (new_arr);
}

int	append_redir(t_token_type type, char *word, t_commands *command)
{
	t_redirections	*copy;

	copy = grow_redirections(command, type, word);
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
	new_argv[i] = ft_strdup(cmd);
	if (!new_argv[i])
	{
		while (i > 0)
			free(new_argv[--i]);
		free(new_argv);
		return (0);
	}
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

	type = (*current)->type;
	*current = (*current)->next;
	if (!*current || (*current)->type != TOK_WORD)
	{
		shell->last_status = 2;
		return (0);
	}
	// TODO: handle expansions;
	// dont just use strdup...
	expanded_word = ft_strdup((*current)->word);
	if (!expanded_word)
		return (0);
	if (!append_redir(type, expanded_word, command))
	{
		free(expanded_word);
		return (0);
	}
	free(expanded_word);
	*current = (*current)->next;
	return (1);
}

int	parse_command(t_token **current, t_commands *command, t_shell *shell)
{
	// char	*expanded_word;
	while (*current && (*current)->type != TOK_PIPE)
	{
		if ((*current)->type == TOK_WORD)
		{
			// TODO: EXPAND COMMAND HERE. DON'T JUST STRDUP
			// expanded_word = ft_strdup((*current)->word);
			// append_word
			if (!append_arg(command, (*current)->word))
				return (0);
			*current = (*current)->next;
		}
		else if (is_redirect((*current)->type))
		{
			if (!handle_redirect(current, command, shell))
				return (0);
		}
		else
			return (0);
	}
	if (command->argc == 0 && command->redirections_count == 0)
		return (0);
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
			// command not found. Maybe do:
			parsed->command_error = 1;
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

	parsed = init_parser(lexer);
	if (!parsed)
		return (NULL);
	current = lexer->head;
	if (!parse_pipeline(&current, parsed, shell, 0))
	{
		//free_parser(parsed);
		parsed->command_error = 1;
		return (NULL);
	}
	return (parsed);
}
