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

/* --- internal helpers for PATH resolution (kept static to avoid .h changes) --- */

/**
 * find_in_path - Returns malloc'd path to executable using PATH, or NULL.
 * @cmd: command name (argv[0]).
 *
 * Notes:
 * - If cmd contains '/', it is checked directly with access(X_OK).
 * - PATH entries may be empty (meaning ".").
 */

static char *find_in_path(const char *cmd)
{
	char *path, *copy, *seg, *p;
	size_t cmdlen, len;
	int i;

	if (!cmd || !*cmd)
		return (NULL);

	for (i = 0; cmd[i]; i++)
		if (cmd[i] == '/')
			break;
	if (cmd[i] == '/')
	{
		if (access(cmd, X_OK) == 0)
		{
			char *ret = (char *)malloc(strlen(cmd) + 1);
			if (!ret) return (NULL);
			strcpy(ret, cmd);
			return (ret);
		}
		return (NULL);
	}

	path = NULL;
	for (i = 0; environ && environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
			break;
		}
	}
	if (!path || !*path)
		return (NULL);

	copy = (char *)malloc(strlen(path) + 1);
	if (!copy)
		return (NULL);
	strcpy(copy, path);
	cmdlen = strlen(cmd);

	seg = copy; p = copy;
	while (1)
	{
		if (*p == ':' || *p == '\0')
		{
			char saved = *p, *dir = seg, *full;

			*p = '\0';
			if (dir[0] == '\0')
				dir = ".";

			len = strlen(dir) + 1 + cmdlen + 1;
			full = (char *)malloc(len);
			if (!full) { free(copy); return (NULL); }

			full[0] = '\0';
			strcpy(full, dir);
			strcat(full, "/");
			strcat(full, cmd);

			if (access(full, X_OK) == 0)
			{
				free(copy);
				return (full);
			}
			free(full);

			if (saved == '\0')
				break;
			seg = p + 1;
		}
		p++;
	}
	free(copy);
	return (NULL);
}

/**
 * run_command - Resolves PATH and forks/execves with argv.
 * @line: raw command line (will be tokenized in-place).
 * @progname: shell name for error messages.
 * Return: 0 on normal path, 1 on internal errors (fork/wait failure).
 *
 * Constraint: do not call fork() if the command does not exist.
 */

int run_command(char *line, char *progname)
{
	pid_t pid;
	int wstatus;
	char **argv = build_argv(line);
	char *cmdpath;

	if (!argv || !argv[0])
	{
		if (argv) free(argv);
		return (0);
	}

	cmdpath = find_in_path(argv[0]);
	if (!cmdpath)
	{
		fprintf(stderr, "%s: %s: not found\n", progname, argv[0]);
		free(argv);
		return (0);
	}

	pid = fork();
	if (pid == -1)
	{
		perror(progname);
		free(cmdpath);
		free(argv);
		return (1);
	}
	if (pid == 0)
	{
		execve(cmdpath, argv, environ);
		perror(progname);
		_exit(127);
	}
	if (waitpid(pid, &wstatus, 0) == -1)
	{
		perror(progname);
		free(cmdpath);
		free(argv);
		return (1);
	}

	free(cmdpath);
	free(argv);
	(void)wstatus;
	return (0);
}
