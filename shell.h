#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;
void exe_com(char *command, char *path);
void execute_command(char *input);
char *read_input(void);

#endif
