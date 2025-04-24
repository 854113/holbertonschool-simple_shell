#include "shell.h"

/**
 * run_com - executes a command
 * @input_line: user input
 */
void run_com(char *input_line)
{
	char *argv[64], *path_env = NULL, *path, *full_path;
	pid_t pid;
	int i = 0, j, found = 0, status;

	if (input_line[0] == '\0')
		return;

	argv[i] = strtok(input_line, " ");
	while (argv[i] != NULL && i < 63)
		argv[++i] = strtok(NULL, " ");

	for (j = 0; environ[j]; j++)
	{
		if (strncmp(environ[j], "PATH=", 5) == 0)
		{
			path_env = strdup(environ[j] + 5);
			break;
		}
	}

	if (path_env && strchr(argv[0], '/') == NULL)
	{
		path = strtok(path_env, ":");
		while (path)
		{
			full_path = malloc(strlen(path) + strlen(argv[0]) + 2);
			if (!full_path)
				return;

			sprintf(full_path, "%s/%s", path, argv[0]);
			if (access(full_path, X_OK) == 0)
			{
				argv[0] = full_path;
				found = 1;
				break;
			}
			free(full_path);
			path = strtok(NULL, ":");
		}
	}

	pid = fork();
	if (pid == 0)
	{
		execve(argv[0], argv, environ);
		perror(argv[0]);
		exit(2);
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			last_exit_status = WEXITSTATUS(status);
	}

	if (found)
		free(argv[0]);
	free(path_env);
}
