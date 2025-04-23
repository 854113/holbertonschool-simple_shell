#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"

/**
 * main - function
 *
 * Return: Always 0
 */

int main(void)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t bytes_read;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("#Simple_Shell$ ");

		bytes_read = getline(&line, &bufsize, stdin);
		if (bytes_read == -1)
		{
			if (isatty(STDIN_FILENO))
			perror("Getline Error");
			break;
		}

		line[strcspn(line, "\n")] = '\0';
		if (strcmp(line, "exit") == 0)
		{
			if (isatty(STDIN_FILENO))
				printf("Exit Shell...\n");
			break;
		}
		execute_command(line);
	}
	free(line);
	line = NULL;
	return (0);
}
