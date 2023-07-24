/* shell_main.c */

#include "shell.h"

char **commands = NULL;
char *line = NULL;
char *shell_name = NULL;
int status = 0;

/**
 * main - The main shell code
 * @argc: Number of arguments passed
 * @argv: Program arguments to be parsed
 *
 * Applies the functions in utils and helpers
 * Implements EOF
 * Prints error on Failure
 * Return: 0 on success
 */
int main(int argc __attribute__((unused)), char **argv)
{
    char **current_command = NULL;
    int i, type_command = 0;
    size_t n = 0;

    signal(SIGINT, handle_sigint);
    shell_name = argv[0];

    while (1)
    {
        _puts("#cisfun$ ");
        if (getline(&line, &n, stdin) == -1)
        {
            free(line);
            exit(status);
        }
        remove_newline(line);
        remove_comment(line);
        commands = tokenizer(line, ";");

        for (i = 0; commands[i] != NULL; i++)
        {
            current_command = tokenizer(commands[i], " ");
            if (current_command[0] == NULL)
            {
                free(current_command);
                break;
            }
            type_command = parse_command(current_command[0]);

            /* initializer -   */
            initializer(current_command, type_command);
            free(current_command);
        }
        free(commands);
    }
    free(line);

    return (status);
}
