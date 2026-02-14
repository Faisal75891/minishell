#include "minishell.h"

extern char	**environ;

int	main(void)
{
	pid_t	pid;
	char	*command;
	char	**arg_list;

	while (1)
	{
		write(1, "$ ", 2);
		command = read_command();
		if (!command)
			continue ;
		arg_list = get_args(command, environ);
		if (!arg_list)
			return (1);
		pid = fork();
		if (pid == 0)
		{
			execve(arg_list[0], arg_list, environ);
			printf("no such file or directory\n");
		}
		else
			wait(NULL);
		free(command);
		free_split(arg_list);
	}
	return (0);
}
