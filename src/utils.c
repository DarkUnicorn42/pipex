
#include "../includes/pipex.h"

void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

char *find_command(char *cmd) {
    char *path_env = getenv("PATH");
    char **paths = ft_split(path_env, ':'); // Assuming ft_split is implemented elsewhere
    char *full_cmd = NULL;
    int i = 0;

    if (!paths)
        error_exit("ft_split");

    while (paths[i]) {
        full_cmd = malloc(strlen(paths[i]) + strlen(cmd) + 2);
        if (!full_cmd)
            error_exit("malloc");
        strcpy(full_cmd, paths[i]);
        strcat(full_cmd, "/");
        strcat(full_cmd, cmd);

        if (access(full_cmd, X_OK) == 0) {
            // Free paths and return the full command path
            for (int j = 0; paths[j]; j++)
                free(paths[j]);
            free(paths);
            return full_cmd;
        }

        free(full_cmd);
        i++;
    }

    // Free paths if command not found
    for (int j = 0; paths[j]; j++)
        free(paths[j]);
    free(paths);

    return NULL;
}

void execute_cmd(char *cmd, char **envp) {
    char **args = ft_split(cmd, ' ');  // Assuming ft_split is implemented elsewhere
    char *full_cmd = NULL;

    if (!args)
        error_exit("ft_split");

    full_cmd = find_command(args[0]);
    if (!full_cmd) {
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
    }

    if (execve(full_cmd, args, envp) == -1) {
        perror("execve");
        exit(EXIT_FAILURE);
    }
}