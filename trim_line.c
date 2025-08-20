#include "shell.h"

/**
 * trim_line - Removes trailing newline and leading/trailing spaces.
 * @line: string from getline
 */

void trim_line(char *line)
{
    size_t i = 0, end, start = 0, k = 0;

    if (!line) return;

    while (line[i] && line[i] != '\n')
        i++;
    if (line[i] == '\n')
        line[i] = '\0';

    if (i == 0) return;

    end = i ? i - 1 : 0;
    while ((int)end >= 0 && (line[end] == ' ' || line[end] == '\t'))
    {
        line[end] = '\0';
        if (end == 0) break;
        end--;
    }
    while (line[start] == ' ' || line[start] == '\t')
        start++;
    if (start)
    {
        while (line[start])
            line[k++] = line[start++];
        line[k] = '\0';
    }
}
