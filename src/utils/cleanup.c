/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/04/06 21:19:49 by fbaras           ###   ########.fr       */
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

// typedef struct s_redirections
// {
// 	char			*target;
// 	t_token_type	type;
// }	t_redirections;

// typedef struct s_commands
// {
// 	char				**argv;
// 	int					argc;
// 	t_redirections		*redirections;
// 	int					redirections_count;
// 	int					heredoc;
// }	t_commands;

// typedef struct s_parsed_result
// {
// 	t_commands	*commands;
// 	int			command_count;
// 	int			command_error;
// }	t_parsed_result;

void	free_parser(t_parsed_result *parser)
{
	int	i;
	int	j;
	int	k;

	if (!parser)
		return ;
	i = 0;
	while (i < parser->command_count)
	{
		j = 0;
		k = 0;
		while (j < parser->commands[i].argc)
		{
			if (parser->commands[i].argv[j])
				free(parser->commands[i].argv[j]);
			j++;
		}
		while (k < parser->commands[i].redirections_count)
		{
			if (parser->commands[i].redirections[k].target)
				free(parser->commands[i].redirections[k].target);
			k++;
		}
		if (parser->commands[i].argv)
			free(parser->commands[i].argv);
		if (parser->commands[i].redirections)
			free(parser->commands[i].redirections);
		i++;
	}
	if (parser->commands)
		free(parser->commands);
	free(parser);
}
