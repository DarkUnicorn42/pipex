#include "../includes/pipex.h"

// The pipex function sets up a pipeline between two commands
void pipex(int fd_in, int fd_out, char **argv, char **envp) {
    int pipe_fd[2];
    pid_t pid1, pid2;

    // Create a pipe, pipe_fd[0] is for reading, pipe_fd[1] is for writing
    if (pipe(pipe_fd) == -1)
        error_exit("pipe");

    // Fork the first child process
    pid1 = fork();
    if (pid1 == -1)
        error_exit("fork");

    if (pid1 == 0) { // Child 1
        // Redirect stdin to fd_in (input file)
        dup2(fd_in, STDIN_FILENO);
        // Redirect stdout to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        // Close pipe ends that are not used by this child
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        // Execute the first command (argv[2])
        execute_cmd(argv[2], envp);
    }

    // Fork the second child process
    pid2 = fork();
    if (pid2 == -1)
        error_exit("fork");

    if (pid2 == 0) { // Child 2
        // Redirect stdin to the read end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);
        // Redirect stdout to fd_out (output file)
        dup2(fd_out, STDOUT_FILENO);
        // Close pipe ends that are not used by this child
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        // Execute the second command (argv[3])
        execute_cmd(argv[3], envp);
    }

    // Close pipe ends in the parent process
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    // Wait for both child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

// The main function sets up file descriptors and calls pipex
int main(int argc, char **argv, char **envp) {
    int fd_in;
    int fd_out;

    // Check the number of arguments
    if (argc != 5) {
        fprintf(stderr, "Usage: %s file1 cmd1 cmd2 file2\n", argv[0]);
        return (EXIT_FAILURE);
    }

    // Open the input file for reading
    fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1)
        error_exit("open infile");

    // Open/Create the output file for writing
    fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1)
        error_exit("open outfile");

    // Call the pipex function to set up the pipeline
    pipex(fd_in, fd_out, argv, envp);

    // Close the file descriptors
    close(fd_in);
    close(fd_out);
    return (EXIT_SUCCESS);
}
