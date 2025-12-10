#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Create a pipe - pipefd[0] is read end, pipefd[1] is write end
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    // Create first child
    int rc1 = fork();
    if (rc1 < 0) {
        // Fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc1 == 0) {
        // First child - this will WRITE to the pipe
        printf("Child 1 (pid: %d): Writing to pipe...\n", getpid());
        
        // Close read end of pipe (we won't use it)
        close(pipefd[0]);
        
        // Redirect stdout to the write end of the pipe
        // After this, anything written to stdout goes to the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);  // Close original fd after duplicating
        
        // Execute a command that writes to stdout
        // This output will go through the pipe to the second child
        execlp("ls", "ls", "-l", NULL);
        
        // If execlp fails
        fprintf(stderr, "execlp failed\n");
        exit(1);
    }

    // Create second child
    int rc2 = fork();
    if (rc2 < 0) {
        // Fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc2 == 0) {
        // Second child - this will READ from the pipe
        printf("Child 2 (pid: %d): Reading from pipe...\n", getpid());
        
        // Close write end of pipe (we won't use it)
        close(pipefd[1]);
        
        // Redirect stdin to the read end of the pipe
        // After this, anything read from stdin comes from the pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);  // Close original fd after duplicating
        
        // Execute a command that reads from stdin
        // This will read the output from child 1
        execlp("wc", "wc", "-l", NULL);
        
        // If execlp fails
        fprintf(stderr, "execlp failed\n");
        exit(1);
    }

    // Parent process
    // Important: Parent must close both ends of the pipe
    // Otherwise, the reading process will never see EOF
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both children to finish
    waitpid(rc1, NULL, 0);
    waitpid(rc2, NULL, 0);

    printf("Parent: Both children have completed\n");
    
    return 0;
}
