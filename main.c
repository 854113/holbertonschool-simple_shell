#include "shelly.h"

/**
 * main - Loop: show prompt -> read line -> execute -> repeat.
 * @argc: argument count (unused).
 * @argv: argument vector; argv[0] is used for error messages.
 * Return: 0 on success, !=0 if internal error occurred.
 */

int main(int argc, char **argv)
{
	char *line = NULL;
	size_t cap = 0;
	ssize_t nread;
	int interactive = isatty(STDIN_FILENO);
	int ret = 0;

	(void)argc;

	while (1)
	{
		if (print_prompt() == -1)
			break;

		nread = getline(&line, &cap, stdin);
		if (nread == -1)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		trim_line(line);
		if (!line[0])
			continue;

		if (run_command(line, argv[0]) != 0)
			ret = 1;
	}

	free(line);
	return (ret);
}

