#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * main - Command interpreter function.
 *
 * Return: 0 in succes
 */

int main(void)
{
        char *command = NULL;
        size_t buffer_size = 0;
        ssize_t char_readed;
        int interact;

        interact = issaty(STDIN_FILENO);

        while (1)
        {
                if (interact)
                        write(STDOUT_FILENO, "$ ", 2);

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

                exe(command)
        }

        free(command);
        return (0);
}
