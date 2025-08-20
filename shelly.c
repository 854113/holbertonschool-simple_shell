#include "shelly.h"

#define PROMPT "#cisfun$ "
#define DELIMS " \t"

/**
 * print_prompt - Prints the prompt if stdin is interactive.
 * Return: 0 on success, -1 if write fails.
 */

int print_prompt(void)
{
	if (isatty(STDIN_FILENO))
	{
		if (write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1) == -1)
			return (-1);
	}
	return (0);
}

/**
 * trim_line - Removes trailing '\n', trims trailing and leading spaces/tabs.
 * @s: null-terminated buffer.
 */

void trim_line(char *s)
{
	size_t i = 0, end, start = 0, k = 0;

	if (!s)
		return;

	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		s[i] = '\0';

	if (i == 0)
		return;
	end = i ? i - 1 : 0;
	while ((int)end >= 0 && (s[end] == ' ' || s[end] == '\t'))
	{
		s[end] = '\0';
		if (end == 0)
			break;
		end--;
	}
	while (s[start] == ' ' || s[start] == '\t')
		start++;
	if (start)
	{
		while (s[start])
			s[k++] = s[start++];
		s[k] = '\0';
	}
}

/**
 * build_argv - Splits a line into tokens (space/tab) to form argv (no realloc).
 * @line: input command line (modified in-place).
 * Return: NULL-terminated argv on success, or NULL on failure/empty.
 */

char **build_argv(char *line)
{
	size_t i, cnt = 0, in = 0, n = 0;
	char **argv;
	char *tok;

	if (!line || !*line)
		return (NULL);

	for (i = 0; line[i]; i++)
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			if (!in) { cnt++; in = 1; }
		}
		else
			in = 0;
	}
	if (cnt == 0)
		return (NULL);

	argv = (char **)malloc((cnt + 1) * sizeof(*argv));
	if (!argv)
		return (NULL);

	for (tok = strtok(line, DELIMS); tok; tok = strtok(NULL, DELIMS))
		argv[n++] = tok;
	argv[n] = NULL;
	return (argv);
}

/**
 * free_argv - Frees argv created by build_argv.
 * @argv: vector to free (only container, tokens are from line buffer).
 */

void free_argv(char **argv)
{
	free(argv);
}

/**
 * run_command - Forks and execves with argv (supports arguments).
 * @line: raw command line (will be tokenized in-place).
 * @progname: shell name for error messages.
 * Return: 0 on success path, 1 on internal errors (fork/wait failure).
 */

int run_command(char *line, char *progname)
{
	pid_t pid;
	int wstatus;
	char **argv = build_argv(line);

	if (!argv || !argv[0])
	{
		free_argv(argv);
		return (0);
	}

	pid = fork();
	if (pid == -1)
	{
		perror(progname);
		free_argv(argv);
		return (1);
	}
	if (pid == 0)
	{
		execve(argv[0], argv, environ);
		perror(progname);
		_exit(127);
	}
	if (waitpid(pid, &wstatus, 0) == -1)
	{
		perror(progname);
		free_argv(argv);
		return (1);
	}
	free_argv(argv);
	(void)wstatus;
	return (0);
}
