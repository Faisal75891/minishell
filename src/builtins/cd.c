/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbaras <fbaras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 00:00:00 by fbaras            #+#    #+#             */
/*   Updated: 2026/02/25 03:25:59 by samamaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: cd builtin
// - cd with no args -> go to HOME
// - cd - -> go to OLDPWD
// - cd <path> -> change directory
// - update PWD and OLDPWD env vars

static char*get_cd_path(t_shell *shell, char **args, char *oldpwd)
{
	char    *path;

	if (!args[1])
	{
		path = get_env_value("HOME", shell->env);
		if (!path)
			ft_putendl_fd("minishell: cd: HOME not set", 2);
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_value("OLDPWD", shell->env);
		if (!path)
			ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
	}
	else
		path = args[1];
	if (!path)
		free(oldpwd);
	return (path);
}

int ms_cd(t_shell *shell, char **args)
{
	char    *path;
	char    *oldpwd;

	oldpwd = getcwd(NULL, 0);
	path = get_cd_path(shell, args, oldpwd);
	if (!path)
		return (1);
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
		ft_putendl_fd(path, 1);
	if (chdir(path) != 0)
	{
		free(oldpwd);
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (1);
	}
	free(oldpwd);
	return (0);
}
