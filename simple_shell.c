#include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include "shell.h"
 
 /**
  * execute_command - execute
  * @command: parameter
  * @path: path
  * Return: void
  */
 
 void exe_com(char *command, char *path)
 {
 	char *path_copy, *dir;
 	char full_path[1024];
 	int found = 0;
 
 	path_copy = strdup(path);
 	if (path_copy == NULL)
 	{
 		perror("PATH Error");
 		return;
 	}
 	dir = strtok(path_copy, ":");
 	while (dir != NULL)
 	{
 		snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
 		if (access(full_path, X_OK) == 0)
 		{
 			found = 1;
 			pid_t pid = fork();
 
 			if (pid == 0)
 			{
 				char *args[] = {command, NULL};
 
 				execve(full_path, args, environ);
 				perror("Execve Error");
 				exit(EXIT_FAILURE);
 			}
 			else if (pid > 0)
 			{
 				wait(NULL);
 			}
 			else
 				perror("Fork Error");
 			break;
 		}
 		dir = strtok(NULL, ":");
 	}
 	free(path_copy);
 	if (!found)
 		printf("Command not found: %s\n", command);
 }
 
 /**
  * main - function
  * @argc: parameter
  * @argv: parameter
  * Return: Always 0
  */
 int main(int argc, char *argv[])
 {
 	ssize_t bytes_read = 0;
 	size_t len = 0;
 	char *line = NULL, *path = getenv("PATH"), *token;
 
 	if (path == NULL)
 	{
 		perror("Failed to get PATH");
 		return (1);
 	}
 
 	while (1)
 	{
 		printf("Simple_Shell$ ");
 		bytes_read = getline(&line, &len, stdin);
 		if (bytes_read == -1)
 		{
 			perror("Read Line Error");
 			break;
 		}
 
 		token = strtok(line, " \n");
 		while (token != NULL)
 		{
 			if (strcmp(token, "exit") == 0)
 			{
 				free(line);
 				return (0);
 			}
 			exe_com(token, path);
 			token = strtok(NULL, " \n");
 		}
 	}
 	free(line);
 	return (0);
 }
 
