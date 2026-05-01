/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 02:03:21 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*expand_redir_target(t_token *token, t_token_type type,
	t_shell *shell)
{
	if (type == TOK_HEREDOC)
		return (ft_strdup(token->word));
	if (token->quote == Q_NONE)
		return (ft_strdup(token->word));
	return (expand_variables(token->word, token->quote, shell));
}

int	handle_redirect(t_token **current, t_commands *command, t_shell *shell)
{
	t_token_type	type;
	t_token			*word_token;
	char			*expanded_word;

	type = (*current)->type;
	*current = (*current)->next;
	word_token = *current;
	if (!word_token || word_token->type != TOK_WORD)
		return (shell->last_status = 2, 0);
	expanded_word = expand_redir_target(word_token, type, shell);
	if (!expanded_word)
		return (shell->last_status = 1, 0);
	if (!append_redir(command, word_token->quote, type, expanded_word))
		return (shell->last_status = 1, free(expanded_word), 0);
	*current = word_token->next;
	return (1);
}
