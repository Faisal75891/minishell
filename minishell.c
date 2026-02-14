#include "minishell.h"

extern char	**environ;

// PATH is ":" seperated
char	*get_full_command(char *command)
{
	int		i;
	char	**paths;

	if (ft_strchr(command, '/'))
		return (command);
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "PATH=", 5));
		paths = ft_split(environ[i], ":");
	}
	while()
	return (NULL);
}

char	*read_command()
{
	char	*command;

	command = get_next_line(0);
	command[ft_strlen(command) - 1] = '\0';
	printf("%s\n", command);
	return (command);
}

char	**get_args(char *command)
{
	char	**arg_list;

	arg_list = ft_split(command, ' ');
	arg_list[0] = get_full_command(arg_list[0]);
	return (arg_list);
}

int	main(void)
{
	pid_t	pid;
	char	*command;
	char	**arg_list;

	for (int i = 0; environ[i]; i++)
	{
		if (!ft_strncmp(environ[i], "PATH=", 5))
			printf("%s\n", environ[i]);
	}

	while (1)
	{
		write(1, "$ ", 2);
		command = read_command();
		arg_list = get_args(command);
		pid = fork();
		if (pid == 0)
		{
			execve(command, arg_list, environ);
			printf("no such file or directory\n");
		}
		else
			wait(NULL);
	}
	return (0);
}
