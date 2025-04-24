#ifndef SHELL_H
#define SHELL_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

extern char **environ;
extern int last_exit_status;

void run_com(char *input_line);
int start_shell(void);

#endif
