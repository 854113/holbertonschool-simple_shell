#ifndef SHELL_H
#define SHELL_H

int start_shell(void);
void run_com(char *input_line);
char *trim_spaces(char *str);
char **tokenize(char *line);
char *find_command(char *cmd, char *path_env);

extern char **environ;

#endif
