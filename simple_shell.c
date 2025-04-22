#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>

void exec_command(char *command);
void display_prompt(int interact);

/**
 * main - Simple shell
 * Return: 0 on success
 */
int main(void)
{
	char *command = NULL;
	size_t buffer_size = 0;
	ssize_t char_readed;
	int interact = isatty(STDIN_FILENO);

	while (1)
	{
		display_prompt(interact);

		char_readed = getline(&command, &buffer_size, stdin);
		if (char_readed == -1)
		{
			if (interact)
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (char_readed > 0 && command[char_readed - 1] == '\n')
			command[char_readed - 1] = '\0';

		if (strlen(command) == 0)
			continue;

		if (strcmp(command, "exit") == 0)
			break;

		exec_command(command);
	}

	free(command);
	return (0);
}

/**
 * display_prompt - Prints shell prompt
 * @interact: if true, shows prompt
 */
void display_prompt(int interact)
{
	if (interact)
		write(STDOUT_FILENO, "$ ", 2);
}

/**
 * exec_command - Forks and executes command
 * @command: command to execute
 */
void exec_command(char *command)
{
	pid_t pid = fork();

	if (pid == 0)
	{
		char *argv[] = {command, NULL};
		execve(command, argv, environ);
		perror("./shell");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
	}
	else
	{
		wait(NULL);
	}
}
