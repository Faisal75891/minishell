/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 02:00:42 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_word(t_token *token, t_shell *shell)
{
	if (token->quote == Q_SINGLE)
		return (ft_strdup(token->word));
	return (expand_variables(token->word, token->quote, shell));
}

static int	append_arg(t_commands *command, char *arg)
{
	char	**new_argv;
	int		i;

	new_argv = malloc(sizeof(char *) * (command->argc + 2));
	if (!new_argv)
		return (0);
	i = 0;
	while (command->argv != NULL && i < command->argc)
	{
		new_argv[i] = command->argv[i];
		i++;
	}
	new_argv[i] = arg;
	new_argv[i + 1] = NULL;
	free(command->argv);
	command->argv = new_argv;
	command->argc++;
	return (1);
}

static int	handle_word(t_token **current, t_commands *command, t_shell *shell)
{
	char	*expanded_word;

	expanded_word = expand_word(*current, shell);
	if (!expanded_word)
	{
		shell->last_status = 1;
		return (0);
	}
	if (!append_arg(command, expanded_word))
	{
		shell->last_status = 1;
		free(expanded_word);
		return (0);
	}
	*current = (*current)->next;
	return (1);
}

int	parse_command(t_token **current, t_commands *command, t_shell *shell)
{
	while (*current && (*current)->type != TOK_PIPE)
	{
		if ((*current)->type == TOK_WORD)
		{
			if (!handle_word(current, command, shell))
				return (0);
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
