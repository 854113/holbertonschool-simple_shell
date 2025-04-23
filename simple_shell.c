#include "shell.h"

#define PROMPT "#cisfun$ "

/**
 * main - Simple UNIX command line interpreter
 * Return: Always 0
 */

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t char_readed;
	pid_t pid;
	int status;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

		char_readed = getline(&line, &len, stdin);
		if (char_readed == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		line[char_readed - 1] = '\0';

		if (line[0] == '\0')
			continue;

		if (strcmp(line, "exit") == 0)
			break;

		pid = fork();
		if (pid == 0)
		{
			char *argv[2];

			argv[0] = line;
			argv[1] = NULL;

			execve(line, argv, environ);
			fprintf(stderr, "./hsh: %s: not found\n", line);
			exit(1);
		}
		else if (pid > 0)
		{
			wait(&status);
		}
		else
		{
			perror("fork");
		}
	}

	free(line);
	return (0);
}
