#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * main - command line interpreter.
 *
 * Return: 0 on success.
 */

#define MAX_CMD_LEN 1024

int main(void)
{
	char command[MAX_CMD_LEN];
	pid_t pid; /* ✅ Declarado al principio del bloque */

	while (1)
	{
		printf("#cisfun$ ");
		fflush(stdout);

		if (!fgets(command, sizeof(command), stdin))
		{
			printf("\n");
			break;
		}

		command[strcspn(command, "\n")] = 0;

		if (*command == '\0')
			continue;

		pid = fork();

		if (pid < 0)
		{
			perror("fork");
			continue;
		}
		else if (pid == 0)
		{
			execlp(command, command, NULL);
			perror("./shell");
			exit(EXIT_FAILURE);
		}

		wait(NULL);
	}

	return (0);
}

