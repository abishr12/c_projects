#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    // Create a pipe for synchronization
    // pipe_fd[0] is for reading, pipe_fd[1] is for writing
    int pipe_fd[2];
    
    if (pipe(pipe_fd) == -1) {
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        close(pipe_fd[0]);  // Close unused read end
        
        printf("hello\n");
        
        // Signal to parent that child is done by writing to pipe
        char signal = 'X';
        write(pipe_fd[1], &signal, 1);
        close(pipe_fd[1]);
        
    } else {
        // Parent process
        close(pipe_fd[1]);  // Close unused write end
        
        // Wait for child's signal by reading from pipe
        // This blocks until child writes, ensuring child prints first
        char signal;
        read(pipe_fd[0], &signal, 1);
        close(pipe_fd[0]);
        
        printf("goodbye\n");
    }
    
    return 0;
}
