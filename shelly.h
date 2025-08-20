#ifndef SHELLY_H
#define SHELLY_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

extern char **environ;

int print_prompt(void);
void trim_line(char *s);
char **build_argv(char *line);
void free_argv(char **argv);
int run_command(char *line, char *progname);

#endif
