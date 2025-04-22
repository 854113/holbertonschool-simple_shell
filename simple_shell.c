#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/**
 * execute_command - fork and execute a command using execve
 * @command: command to execute (full path)
 */

void execute_command(char *command)
{
	pid_t pid;
	char *argv[] = {command, NULL};
	extern char **environ;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}

	if (pid == 0)
	{
		if (execve(command, argv, environ) == -1)
		{
			perror("./shell");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(NULL);
	}
}

/**
 * main - Command interpreter function.
 *
 * Return: 0 on success
 */
int main(void)
{
	char *command = NULL;
	size_t buffer_size = 0;
	ssize_t char_readed;
	int interact;

	interact = isatty(STDIN_FILENO);

	while (1)
	{
		if (interact)
			write(STDOUT_FILENO, "$ ", 2);

		char_readed = getline(&command, &buffer_size, stdin);

		if (char_readed == -1)
		{
			if (interact)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (char_readed > 0 && command[char_readed - 1] == '\n')
			command[char_readed - 1] = '\0';

		if (strlen(command) == 0)
			continue;

		if (strcmp(command, "exit") == 0)
			break;

		execute_command(command);
	}

	free(command);
	return (0);
}
