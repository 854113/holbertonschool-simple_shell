#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * trim_spaces - Trim leading and trailing spaces from a string
 * @str: The string to trim
 *
 * Return: Pointer to the trimmed string
 */
char *trim_spaces(char *str)
{
	char *end;

	while (*str == ' ' || *str == '\t')
		str++;

	if (*str == 0)
		return (str);

	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t'))
		end--;

	end[1] = '\0';

	return (str);
}

/**
 * tokenize_input - Split input into tokens
 * @input_line: The input line to tokenize
 * @argv: Array to store tokens
 *
 * Return: Number of tokens or -1 if empty
 */
int tokenize_input(char *input_line, char **argv)
{
	int i = 0;

	argv[i] = strtok(input_line, " ");
	if (argv[0] == NULL)
		return (-1);

	while (argv[i] != NULL && i < 63)
		argv[++i] = strtok(NULL, " ");
	argv[i] = NULL;

	return (i);
}

/**
 * find_in_path - Find command in PATH
 * @command: Command to find
 * @environ: Environment variables
 *
 * Return: Full path to command or NULL if not found
 */
char *find_in_path(char *command, char **environ)
{
	char *path_env = NULL, *path, *full_path;
	int j;

	for (j = 0; environ[j]; j++)
	{
		if (strncmp(environ[j], "PATH=", 5) == 0)
		{
			path_env = environ[j] + 5;
			break;
		}
	}

	if (!path_env || strchr(command, '/') != NULL)
		return (NULL);

	path = strtok(path_env, ":");
	while (path)
	{
		full_path = malloc(strlen(path) + strlen(command) + 2);
		if (!full_path)
			return (NULL);

		sprintf(full_path, "%s/%s", path, command);

		if (access(full_path, X_OK) == 0)
			return (full_path);

		free(full_path);
		path = strtok(NULL, ":");
	}

	return (NULL);
}

/**
 * execute_command - Execute the command with fork and execve
 * @command: Command to execute
 * @argv: Arguments for the command
 *
 * Return: Exit status of the command
 */
int execute_command(char *command, char **argv)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork Error");
		return (1);
	}

	if (pid == 0)
	{
		if (execve(command, argv, environ) == -1)
		{
			perror(command);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);
		return (WEXITSTATUS(status));
	}

	return (0);
}

/**
 * run_com - Execute a command
 * @input_line: User input line
 *
 * Return: void
 */
void run_com(char *input_line)
{
	char *argv[64];
	char *cmd_path = NULL;
	int tokens;
	int replaced = 0;

	input_line = trim_spaces(input_line);
	if (*input_line == '\0')
		return;

	tokens = tokenize_input(input_line, argv);
	if (tokens == -1)
		return;

	if (access(argv[0], X_OK) != 0)
	{
		cmd_path = find_in_path(argv[0], environ);
		if (cmd_path)
		{
			argv[0] = cmd_path;
			replaced = 1;
		}
	}

	execute_command(argv[0], argv);

	if (replaced)
		free(cmd_path);
}
