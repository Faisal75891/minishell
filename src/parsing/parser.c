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

static int	lexer_invalid(t_lex_result *lexer, t_shell *shell)
{
	if (!lexer || lexer->error > 0 || lexer->count <= 0 || !lexer->head)
	{
		shell->last_status = 1;
		return (1);
	}
	return (0);
}

static int	parse_pipeline(t_token **current, t_parsed_result *parsed,
	t_shell *shell)
{
	int	i;

	i = 0;
	while (i < parsed->command_count)
	{
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
		}
		i++;
	}
	return (1);
}

t_parsed_result	*parser(t_lex_result *lexer, t_shell *shell)
{
	t_parsed_result	*parsed;
	t_token			*current;

	if (lexer_invalid(lexer, shell))
		return (NULL);
	parsed = init_parser(lexer);
	if (!parsed)
	{
		shell->last_status = 1;
		return (NULL);
	}
	current = lexer->head;
	if (!parse_pipeline(&current, parsed, shell))
	{
		free_parser(parsed);
		return (NULL);
	}

	return (parsed);
}
