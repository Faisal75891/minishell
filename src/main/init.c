/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 01:45:04 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/08 01:19:44 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// i didn't where to put this
// env_utils.c? or env.c? or here.
char	**copy_env(char **envp)
{
	char	**env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env = malloc ((i + 1) * sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
		{
			while (i > 0)
				free(env[i--]);
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	init_lexer_and_shell(t_lex_result **lexer,
	t_shell **shell, char **envp)
{
	*shell = init_shell(envp);
	if (!(*shell))
		return (0);
	*lexer = init_lexer();
	if (!(*lexer))
	{
		free_split((*shell)->env);
		free(*shell);
		return (0);
	}
	return (1);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc (sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->last_status = 0;
	return (shell);
}
