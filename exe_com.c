#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

/**
 * execute_command - execute a command
 * @line: parameter
 *
 * Return: void
 */

void execute_command(char *line)
{
	char *argv[64], command_path[256], *envp[] = {"PATH=/bin:/usr/bin", NULL};
	pid_t pid;
	int i = 0;

	argv[i] = strtok(line, " ");
	while (argv[i] != NULL && i < 63)
		argv[++i] = strtok(NULL, " ");

	if (argv[0][0] != '/')
		sprintf(command_path, "/bin/%s", argv[0]);
	else
		strncpy(command_path, argv[0], sizeof(command_path));

	pid = fork();
	if (pid == -1)
	{
		perror("Fork Error");
		return;
	}

	if (pid == 0)
	{
		if (execve(command_path, argv, envp) == -1)
		{
			perror("Command Not Found");
		exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(NULL);
	}
}
