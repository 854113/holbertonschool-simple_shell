#include "shell.h"

/**
 * main - Simple shell loop
 */

int main(int ac, char **av)
{
    char *line = NULL;
    size_t cap = 0;
    ssize_t n;
    unsigned int lineno = 0;
    int status = 0;

    (void)ac;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(1, "#cisfun$ ", 9);

        n = getline(&line, &cap, stdin);
        if (n == -1) { if (isatty(STDIN_FILENO)) write(1, "\n", 1); break; }

        lineno++;
        trim_line(line);
        if (*line) status = run_command(line, av[0], lineno);
    }
    free(line);
    return status;
}
