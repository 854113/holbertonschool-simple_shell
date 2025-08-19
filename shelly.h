#ifndef SHELLY_H
#define SHELLY_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

int print_prompt(void);
void trim_line(char *s);
int run_command(char *cmd, char *progname);

#endif /* SHELLY_H */

