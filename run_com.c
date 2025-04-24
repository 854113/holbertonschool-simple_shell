#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

/**
 * run_com - execute a command
 * @input_line: user input line
 *
 * Return: void
 */
void run_com(char *input_line)
{
	char *argv[64];
	pid_t pid;
	int i = 0;

	argv[i] = strtok(input_line, " ");
	while (argv[i] != NULL && i < 63)
		argv[++i] = strtok(NULL, " ");

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
}
