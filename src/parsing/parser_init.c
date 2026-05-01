/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 02:00:42 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_commands(t_lex_result *lexer)
{
	t_token	*current;
	int		count;

	count = 1;
	current = lexer->head;
	while (current)
	{
		if (current->type == TOK_PIPE)
			count++;
		current = current->next;
	}
	return (count);
}

static t_commands	*init_commands(int command_count)
{
	t_commands	*commands;
	int			i;

	commands = malloc(sizeof(t_commands) * command_count);
	if (!commands)
		return (NULL);
	i = 0;
	while (i < command_count)
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

	parser = malloc(sizeof(t_parsed_result));
	if (!parser)
		return (NULL);
	parser->command_count = count_commands(lexer);
	parser->command_error = 0;
	parser->commands = init_commands(parser->command_count);
	if (!parser->commands)
	{
		free(parser);
		return (NULL);
	}
	return (parser);
}
