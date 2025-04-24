#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * start_shell - Entry point of command interpreter.
 *
 * Return: 0
 */

int start_shell(void)
{
	char *input_line = NULL;
	size_t len = 0;
	ssize_t chars_read;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("#Simple_Shell> ");

		chars_read = getline(&input_line, &len, stdin);
		if (chars_read == -1)
		{
			if (isatty(STDIN_FILENO))
				perror("Error Reading Line");
			break;
		}

		input_line[strcspn(input_line, "\n")] = 0;

		if (strcmp(input_line, "exit") == 0)
		{
			if (isatty(STDIN_FILENO))
				printf("Exiting Shell...\n");
			break;
		}

		run_com(input_line);
	}

	free(input_line());
	return (0);
}

int main(void)

{
	return (start_shell);
}
