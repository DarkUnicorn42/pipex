#include "../includes/pipex.h"

void pipex(int fd_in, int fd_out, char **argv, char **envp)
{
	int	pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	
	if (pipe(pipe_fd) == -1)
		error_exit("err_pipe");
	pid1 = fork();
	if (pid1 == -1)
		error_exit("err_fork");	
	if (pid1 == 0)
	{
		dup2(fd_in, STDIN_FILENO);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		execute_cmd(argv[2], envp);
	}
	pid2 = fork();
	if (pid2 == -1)
	    error_exit("err_fork");	
	if (pid2 == 0)
	{
	    dup2(pipe_fd[0], STDIN_FILENO);
	    dup2(fd_out, STDOUT_FILENO);
	    close(pipe_fd[0]);
	    close(pipe_fd[1]);
	    execute_cmd(argv[3], envp);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}	

int main(int argc, char **argv, char **envp)
{
	int fd_in;
	int fd_out;	
	if (argc != 5)
		error_exit("err_arg_count");
	fd_in = open(argv[1], O_RDONLY);
	if (fd_in == -1)
	    error_exit("err_infile");
	fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	    error_exit("err_outfile");
	pipex(fd_in, fd_out, argv, envp);
	close(fd_in);
	close(fd_out);
	return (EXIT_SUCCESS);
}