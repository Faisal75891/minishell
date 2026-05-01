/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samamaev <samamaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/02 02:20:30 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

// i didn't know where to put this lol
int	split_len(char **arr)
{
	int	len;

	len = 0;
	if (!arr)
		return (0);
	while (arr[len])
		len++;
	return (len);
}

static void	free_cmd_argv(t_commands *cmd)
{
	int	j;

	if (!cmd || !cmd->argv)
		return ;
	j = 0;
	while (j < cmd->argc)
	{
		free(cmd->argv[j]);
		j++;
	}
	free(cmd->argv);
	cmd->argv = NULL;
}

static void	free_cmd_redirections(t_commands *cmd)
{
	int	k;

	if (!cmd || !cmd->redirections)
		return ;
	k = 0;
	while (k < cmd->redirections_count)
	{
		free(cmd->redirections[k].target);
		k++;
	}
	free(cmd->redirections);
	cmd->redirections = NULL;
}

void	free_parser(t_parsed_result *parser)
{
	int	i;

	if (!parser)
		return ;
	i = 0;
	while (i < parser->command_count)
	{
		free_cmd_argv(&parser->commands[i]);
		free_cmd_redirections(&parser->commands[i]);
		i++;
	}
	if (parser->commands)
		free(parser->commands);
	free(parser);
}
