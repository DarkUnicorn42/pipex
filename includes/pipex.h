
#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libft/libft.h"

void error_exit(const char *msg);
void execute_cmd(char *cmd, char **envp);
void pipex(int fd_in, int fd_out, char **argv, char **envp);
char *find_command(char *cmd);


#endif