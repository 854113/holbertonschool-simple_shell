#ifndef SHELL_H
#define SHELL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute_command(char *command);
char **parse_input(char *input);
void free_array(char **array);
int builtin_exit(char **args);

#endif
