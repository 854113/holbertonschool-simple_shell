#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

extern char **environ;

/**
 * run_com - execute a command
 * @input_line: user input line
 *
 * Return: void
 */
void run_com(char *input_line)
{
	char *argv[64];
	char *path_env = NULL, *path, *full_path;
	pid_t pid;
	int i = 0, j;

	argv[i] = strtok(input_line, " ");
	if (argv[0] == NULL)
		return;

	while (argv[i] != NULL && i < 63)
		argv[++i] = strtok(NULL, " ");

	for (j = 0; environ[j]; j++)
	{
		if (strncmp(environ[j], "PATH=", 5) == 0)
		{
			path_env = environ[j] + 5;
			break;
		}
	}

	if (strchr(argv[0], '/') == NULL && path_env)
	{
		path = strtok(path_env, ":");
		while (path)
		{
			full_path = malloc(strlen(path) + strlen(argv[0]) + 2);
			if (!full_path)
				return;

			sprintf(full_path, "%s/%s", path, argv[0]);

			if (access(full_path, X_OK) == 0)
			{
				argv[0] = full_path;
				break;
			}

			free(full_path);
			path = strtok(NULL, ":");
		}
	}

	pid = fork();
	if (pid == -1)
	{
		perror("Fork Error");
		return;
	}

	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			perror("Command Not Found");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(NULL);
	}

	if (strchr(argv[0], '/') && access(argv[0], X_OK) == 0)
		free(argv[0]);
}
