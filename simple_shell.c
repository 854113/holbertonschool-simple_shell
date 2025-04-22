#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

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
	pid_t pid;

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

		pid = fork();
		if (pid == 0)
		{
			char *argv[] = {command, NULL};
			execve(command, argv, environ);
			perror("./shell");
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
		{
			perror("fork");
		}
		else
		{
			wait(NULL);
		}
	}

	free(command);
	return (0);
}
