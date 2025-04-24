#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

/**
 * trim_spaces - remove leading and trailing spaces
 * @str: input string
 * Return: trimmed string
 */
char *trim_spaces(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;

	if (*str == '\0')
		return (str);

	char *end = str + strlen(str) - 1;

	while (end > str && (*end == ' ' || *end == '\t'))
		end--;

	end[1] = '\0';

	return (str);
}

/**
 * tokenize - split input into arguments
 * @line: input string
 * Return: array of tokens
 */
char **tokenize(char *line)
{
	static char *argv[64];
	int i = 0;

	argv[i] = strtok(line, " ");
	if (argv[i] == NULL)
		return (NULL);

	while (argv[i] != NULL && i < 63)
		argv[++i] = strtok(NULL, " ");

	return (argv);
}

/**
 * find_command - locate full path for command
 * @cmd: command name
 * @path_env: PATH string
 * Return: full path (malloc'd) or NULL
 */
char *find_command(char *cmd, char *path_env)
{
	char *path, *full_path;

	if (strchr(cmd, '/') != NULL)
		return (strdup(cmd));

	path = strtok(path_env, ":");
	while (path)
	{
		full_path = malloc(strlen(path) + strlen(cmd) + 2);
		if (!full_path)
			return (NULL);

		sprintf(full_path, "%s/%s", path, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);

		free(full_path);
		path = strtok(NULL, ":");
	}

	return (NULL);
}

/**
 * run_com - execute a command
 * @input_line: user input line
 * Return: void
 */
void run_com(char *input_line)
{
	char *path_env = NULL, *cmd_path;
	char **argv;
	pid_t pid;
	int j;

	input_line = trim_spaces(input_line);
	if (*input_line == '\0')
		return;

	argv = tokenize(input_line);
	if (!argv || !argv[0])
		return;

	for (j = 0; environ[j]; j++)
	{
		if (strncmp(environ[j], "PATH=", 5) == 0)
		{
			path_env = strdup(environ[j] + 5);
			break;
		}
	}

	cmd_path = find_command(argv[0], path_env);
	if (!cmd_path)
	{
		perror(argv[0]);
		free(path_env);
		return;
	}

	argv[0] = cmd_path;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork Error");
		free(cmd_path);
		free(path_env);
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
		wait(NULL);

	free(cmd_path);
	free(path_env);
}
