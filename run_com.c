#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

extern char **environ;

char *trim_spaces(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;

	if (*str == 0)
		return str;

	char *end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;

	end[1] = '\0';

	return str;
}

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
	int i = 0, j, replaced = 0;

	input_line = trim_spaces(input_line);
	if (*input_line == '\0')
		return;

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
				replaced = 1;
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
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(NULL);
	}

	if (replaced)
		free(argv[0]);
}
