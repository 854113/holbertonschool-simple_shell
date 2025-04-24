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
	char *command = NULL;
	size_t buffer_size = 0;
	ssize_t b_readed;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("#Simple_Shell$ ");

		b_readed = getline(&command, &buffer_size, stdin);
		if (b_readed == -1)
		{
			if (isatty(STDIN_FILENO))
			perror("Getline Error");
			break;
		}

		command[strcspn(command, "\n")] = '\0';
		if (strcmp(command, "exit") == 0)
		{
			if (isatty(STDIN_FILENO))
				printf("Exit Shell...\n");
			break;
		}
		exe_com(command);
	}
	free(command);
	command = NULL;
	return (0);
}
