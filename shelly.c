#include "shelly.h"
#include <string.h>

#define PROMPT "#cisfun$ "

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
 * trim_line - Removes trailing '\n' and leading spaces/tabs in-place.
 * @s: buffer terminated by '\0'.
 */

void trim_line(char *s)
{
	size_t i = 0, j = 0;

	if (!s)
		return;

	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		s[i] = '\0';

	for (i = 0; s[i] == ' ' || s[i] == '\t'; i++)
		;

	if (i)
	{
		while (s[i])
			s[j++] = s[i++];
		s[j] = '\0';
	}
}

/**
 * run_command - Creates a child process and executes the command.
 * @cmd: path or name of the executable (no arguments).
 * @progname: argv[0] of the shell, used in error messages.
 * Return: 0 if executed, 1 if internal error occurred.
 */

int run_command(char *cmd, char *progname)
{
	pid_t pid;
	int wstatus;
	char *argv_exec[2];

	if (!cmd || !*cmd)
		return (0);

	argv_exec[0] = cmd;
	argv_exec[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror(progname);
		return (1);
	}
	if (pid == 0)
	{
		execve(cmd, argv_exec, environ);
		perror(progname);
		_exit(127);
	}
	if (waitpid(pid, &wstatus, 0) == -1)
	{
		perror(progname);
		return (1);
	}
	(void)wstatus;
	return (0);
}
