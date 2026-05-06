/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/05/01 23:47:14 by fbaras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "../../include/minishell.h"

// get environment variable value by name (NAME without '=')
char	*get_env_value(const char *name, char **env)
{
	int	len;
	int	i;

	if (!name || !*name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], name, len) && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*ms_env_make_entry(const char *name, const char *value)
{
	char	*entry;
	char	*tmp;

	entry = ft_strjoin(name, "=");
	if (!entry)
		return (NULL);
	if (value)
	{
		tmp = ft_strjoin(entry, value);
		free(entry);
		entry = tmp;
	}
	return (entry);
}

static int	ms_env_update_existing(t_shell *shell, const char *name,
	int keylen, char *entry)
{
	int	i;

	i = 0;
	while (shell->env && shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], name, keylen)
			&& shell->env[i][keylen] == '=')
		{
			free(shell->env[i]);
			shell->env[i] = entry;
			return (0);
		}
		i++;
	}
	return (1);
}

static int	ms_env_append_new(t_shell *shell, char *entry)
{
	int		count;
	char	**new_env;

	count = 0;
	while (shell->env && shell->env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (free(entry), 1);
	count = -1;
	while (shell->env && shell->env[++count])
		new_env[count] = shell->env[count];
	new_env[count] = entry;
	new_env[count + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (0);
}

int	set_env_value(t_shell *shell, const char *name, const char *value)
{
	int		keylen;
	char	*entry;

	if (!shell || !name || !*name)
		return (1);
	keylen = 0;
	while (name[keylen] && name[keylen] != '=')
		keylen++;
	if (keylen <= 0)
		return (1);
	entry = ms_env_make_entry(name, value);
	if (!entry)
		return (1);
	if (!ms_env_update_existing(shell, name, keylen, entry))
		return (0);
	return (ms_env_append_new(shell, entry));
}
