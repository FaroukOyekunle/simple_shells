/* shell_utils.c */

#include "shell.h"

/**
 * parse_command - Determines the type of the command.
 * @command: The command to be parsed.
 *
 * Return: A constant representing the type of the command.
 *     EXTERNAL_COMMAND (1) for commands like /bin/ls,
 *     INTERNAL_COMMAND (2) for commands like exit, env,
 *     PATH_COMMAND (3) for commands found in the PATH,
 *     INVALID_COMMAND (-1) for invalid commands.
 */
int parse_command(char *command)
{
    int i;
    char *internal_commands[] = {"env", "exit", NULL};
    char *path_check = NULL;

    for (i = 0; command[i] != '\0'; i++)
    {
        if (command[i] == '/')
            return (EXTERNAL_COMMAND);
    }

    for (i = 0; internal_commands[i] != NULL; i++)
    {
        if (_strcmp(command, internal_commands[i]) == 0)
            return (INTERNAL_COMMAND);
    }

    /* Check if the command is found in the PATH */
    path_check = check_path(command);
    if (path_check != NULL)
    {
        free(path_check);
        return (PATH_COMMAND);
    }

    return (INVALID_COMMAND);
}

/**
 * execute_command - Executes a command based on its type.
 * @tokenized_command: The tokenized form of the command.
 * @command_type: The type of the command.
 *
 * Return: void
 */
void execute_command(char **tokenized_command, int command_type)
{
    void (*command_func)(char **command);

    if (command_type == EXTERNAL_COMMAND)
    {
        if (execve(tokenized_command[0], tokenized_command, NULL) == -1)
        {
            perror(_getenv("PWD"));
            exit(2);
        }
    }
    else if (command_type == PATH_COMMAND)
    {
        if (execve(check_path(tokenized_command[0]), tokenized_command, NULL) == -1)
        {
            perror(_getenv("PWD"));
            exit(2);
        }
    }
    else if (command_type == INTERNAL_COMMAND)
    {
        command_func = get_internal_command(tokenized_command[0]);
        command_func(tokenized_command);
    }
    else if (command_type == INVALID_COMMAND)
    {
        print_error(shell_name, tokenized_command[0]);
        status = 127;
    }
}

/**
 * check_path - Checks if a command is found in the PATH.
 * @command: The command to be checked.
 *
 * Return: The full path where the command is found, or NULL if not found.
 */
char *check_path(char *command)
{
    char **path_tokens = NULL;
    char *temp_path, *full_path, *path_copy;
    char *path_env = _getenv("PATH");
    int i;

    if (path_env == NULL || _strlen(path_env) == 0)
        return (NULL);

    path_copy = _strdup(path_env);
    path_tokens = tokenizer(path_copy, ":");

    for (i = 0; path_tokens[i] != NULL; i++)
    {
        temp_path = _strcat(path_tokens[i], "/");
        full_path = _strcat(temp_path, command);

        if (access(full_path, F_OK) == 0)
        {
            free(temp_path);
            free(path_copy);
            free_tokens(path_tokens);
            return (full_path);
        }

        free(full_path);
        free(temp_path);
    }

    free(path_copy);
    free_tokens(path_tokens);
    return (NULL);
}

/**
 * get_internal_command - Retrieves the function for a built-in command.
 * @command: The command to check against the built-in commands.
 *
 * Return: A pointer to the proper function, or NULL if not found.
 */
void (*get_internal_command(char *command))(char **)
{
    int i;
    function_map internal_command_mapping[] = {
        {"env", shell_env}, {"exit", shell_exit}
    };

    for (i = 0; i < 2; i++)
    {
        if (_strcmp(command, internal_command_mapping[i].command_name) == 0)
            return (internal_command_mapping[i].func);
    }

    return (NULL);
}

/**
 * _getenv - Gets the value of an environment variable.
 * @name: The name of the environment variable.
 *
 * Return: The value of the variable as a string.
 */
char *_getenv(char *name)
{
    char **env;
    char *pair_ptr;
    char *name_copy;

    for (env = environ; *env != NULL; env++)
    {
        for (pair_ptr = *env, name_copy = name;
             *pair_ptr == *name_copy; pair_ptr++, name_copy++)
        {
            if (*pair_ptr == '=')
                break;
        }

        if ((*pair_ptr == '=') && (*name_copy == '\0'))
            return (pair_ptr + 1);
    }

    return (NULL);
}
