#include "shell.h"

int last_exit_status = 0;

/**
 * start_shell - main loop of the shell
 * Return: 0
 */

int start_shell(void)
{
	char *input_line = NULL;
	size_t len = 0;
	ssize_t nread;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "$ ", 2);

		nread = getline(&input_line, &len, stdin);
		if (nread == -1)
			break;

		input_line[strcspn(input_line, "\n")] = '\0';

		if (strcmp(input_line, "exit") == 0)
		{
			free(input_line);
			exit(last_exit_status);
		}

		run_com(input_line);
	}

	free(input_line);
	return (0);
}
