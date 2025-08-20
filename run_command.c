#include "shelly.h"

static char **make_argv(char *line)
{
	int count = 0; 
	int i = 0;
	char *copy; 
	char *tok;
	char **argv;

	copy = strdup(line);
	if (!copy)
		return (NULL);

	tok = strtok(copy, " \t");
	while (tok)
	{
		count++;
		tok = strtok(NULL, " \t");
	}
	free(copy);

	if (count == 0)
		return (NULL);

	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);

	tok = strtok(line, " \t");
	while (tok)
	{
		argv[i++] = tok;
		tok = strtok(NULL, " \t");
	}
	argv[i] = NULL;
	return (argv);
}

static char *find_cmd(char *cmd)
{
	int i;
	char *path = NULL, *copy, *dir, *p, *full;
	size_t len, cmdlen;

	if (!cmd || !*cmd)
		return (NULL);

	for (i = 0; cmd[i]; i++)
		if (cmd[i] == '/')
			break;
	if (cmd[i] == '/')
		return ((access(cmd, X_OK) == 0) ? strdup(cmd) : (NULL));

	for (i = 0; environ[i]; i++)
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
			break;
		}
	if (!path)
		return (NULL);

	copy = strdup(path);
	if (!copy)
		return (NULL);

	cmdlen = strlen(cmd);
	dir = copy;
	p = copy;
	while (1)
	{
		if (*p == ':' || *p == '\0')
		{
			char save = *p;

			*p = '\0';
			if (*dir == '\0')
				dir = ".";
			len = strlen(dir) + 1 + cmdlen + 1;
			full = malloc(len);
			if (!full)
			{
				free(copy);
				return (NULL);
			}
			snprintf(full, len, "%s/%s", dir, cmd);
			if (access(full, X_OK) == 0)
			{
				free(copy);
				return (full);
			}
			free(full);
			if (save == '\0')
				break;
			dir = p + 1;
		}
		p++;
	}
	free(copy);
	return (NULL);
}

/**
 * run_command - Execute a command with PATH support.
 * @line: command line (will be tokenized)
 * @prog: name of shell (argv[0])
 * @lineno: input line number for errors
 *
 * Return: exit status of command or error code
 */

int run_command(char *line, char *prog, unsigned int lineno)
{
    char **argv = make_argv(line);
    char *cmd;
    pid_t pid;
    int status;

    if (!argv || !argv[0]) { free(argv); return 0; }

    if (strcmp(argv[0], "exit") == 0)
    {
        free(argv);
        free(line);
        exit(0);
    }

    cmd = find_cmd(argv[0]);
    if (!cmd)
    {
        fprintf(stderr, "%s: %u: %s: not found\n", prog, lineno, argv[0]);
        free(argv);
        return 127;
    }

    pid = fork();
    if (pid == 0) { execve(cmd, argv, environ); perror(prog); _exit(127); }
    if (pid < 0) { perror(prog); free(cmd); free(argv); return 1; }

    waitpid(pid, &status, 0);
    free(cmd);
    free(argv);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
