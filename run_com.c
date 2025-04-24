#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * run_com - execute a command
 * @input_line: user input line
 */
void run_com(char *input_line)
{
	char *argv[64], *path_env = NULL, *path, *full_path;
	pid_t pid;
	int i = 0, j, found = 0;

	input_line[strcspn(input_line, "\n")] = '\0';

	if (input_line[0] == '\0')
		return;

	argv[i] = strtok(input_line, " ");
	while (argv[i] != NULL && i < 63)
		argv[++i] = strtok(NULL, " ");

	for (j = 0; environ[j]; j++)
	{
		if (strncmp(environ[j], "PATH=", 5) == 0)
		{
			path_env = strdup(environ[j] + 5);
			break;
		}
	}

	if (path_env && strchr(argv[0], '/') == NULL)
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
				found = 1;
				break;
			}
			free(full_path);
			path = strtok(NULL, ":");
		}
	}

	pid = fork();
	if (pid == 0)
		execve(argv[0], argv, environ), perror(argv[0]), exit(1);
	else
		wait(NULL);

	if (found)
		free(argv[0]);
	free(path_env);
}
