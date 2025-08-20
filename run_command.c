#include "shell.h"

static char **make_argv(char *line)
{
    char **argv = NULL, *tok;
    size_t n = 0;

    for (tok = strtok(line, " \t"); tok; tok = strtok(NULL, " \t"))
    {
        argv = realloc(argv, sizeof(char *) * (n + 2));
        if (!argv) return (NULL);
        argv[n++] = tok;
    }
    if (argv) argv[n] = NULL;
    return (argv);
}

static char *find_cmd(char *cmd)
{
    char *path, *p, *dir, *full;
    size_t len;

    if (strchr(cmd, '/'))
        return access(cmd, X_OK) == 0 ? strdup(cmd) : NULL;

    path = getenv("PATH");
    if (!path) return NULL;
    path = strdup(path);

    for (dir = strtok(path, ":"); dir; dir = strtok(NULL, ":"))
    {
        len = strlen(dir) + strlen(cmd) + 2;
        full = malloc(len);
        if (!full) break;
        snprintf(full, len, "%s/%s", dir, cmd);
        if (access(full, X_OK) == 0) { free(path); return full; }
        free(full);
    }
    free(path);
    return NULL;
}

/**
 * run_command - Execute a command with PATH support.
 * @line: command line (will be tokenized)
 * @prog: name of shell (argv[0])
 * @lineno: input line number for errors
 */

int run_command(char *line, char *prog, unsigned int lineno)
{
    char **argv = make_argv(line);
    char *cmd;
    pid_t pid;
    int status;

    if (!argv || !argv[0]) { free(argv); return 0; }

    cmd = find_cmd(argv[0]);
    if (!cmd)
    {
        fprintf(stderr, "%s: %u: %s: not found\n", prog, lineno, argv[0]);
        free(argv);
        return 127;
    }

    pid = fork();
    if (pid == 0) { execve(cmd, argv, environ); perror(prog); _exit(127); }
    if (pid < 0) { perror(prog); free(cmd); free(argv); return 1; }

    waitpid(pid, &status, 0);
    free(cmd);
    free(argv);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
