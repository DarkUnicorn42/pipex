
#include "../includes/pipex.h"

void error_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

char *find_command(char *cmd) {
    char *path_env = getenv("PATH");
    char **paths = ft_split(path_env, ':');
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

        if (access(full_cmd, X_OK) == 0)
	  {
            for (int j = 0; paths[j]; j++)
                free(paths[j]);
            free(paths);
            return full_cmd;
        }
        free(full_cmd);
        i++;
    }
    for (int j = 0; paths[j]; j++)
        free(paths[j]);
    free(paths);
    return (NULL);
}

void execute_cmd(char *cmd, char **envp)
{
	char **args;
	char *full_cmd;

	full_cmd = NULL;
	args = ft_split(cmd, ' ');
	if (!args)
		error_exit("ft_split");	
	full_cmd = find_command(args[0]);
	if (!full_cmd)
		error_exit("cmd not found");
	if (execve(full_cmd, args, envp) == -1)
		error_exit("execve err");
}
