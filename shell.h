#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>

#define MAX_ARGS 64
#define BUFFER_SIZE 64

extern char **environ;
int tokenize(char *input, char *args[]);
int _strlen(char *s);
char *_strcat(char *dest, char *src);
int _strcmp(char *s1, char *s2);
int _atoi(char *s);
void print_number(int n);
int _putchar(char c);
void _puts(char *str);
char *_strcpy(char *dest, char *src);
char *get_cmd(char *line);
char *_strdup(char *str);

/* Built-in commands */
int shell_cd(char **args);
int shell_exit(char **args);
int shell_env(char **args);
void env(char **tokenized_command);
void quit(char **tokenized_command);

/* Utility functions */
void execute_external_command(char **args);
void handle_sigint(int signum);
char *_getenv(char *name);

/* Helper functions */
void remove_newline(char *str);
void remove_comment(char *str);

/* Main functions */
int execute_command(char **args);

#endif /* SHELL_H */
