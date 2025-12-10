/*
 * Question 5: Understanding wait()
 * 
 * This program explores:
 * 1. What wait() returns in the parent process
 * 2. What happens when wait() is called in the child process
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("Parent process (PID: %d) starting...\n\n", (int) getpid());
    
    int rc = fork();
    
    if (rc < 0) {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        exit(1);
        
    } else if (rc == 0) {
        // Child process
        printf("CHILD: I am the child process (PID: %d)\n", (int) getpid());
        printf("CHILD: My parent's PID is: %d\n", (int) getppid());
        
        // Try calling wait() in the child
        printf("CHILD: Calling wait() in the child process...\n");
        int wait_result = wait(NULL);
        
        printf("CHILD: wait() returned: %d\n", wait_result);
        if (wait_result == -1) {
            printf("CHILD: wait() returned -1 because I have no children to wait for!\n");
        }
        
        printf("CHILD: Exiting with status 42...\n\n");
        exit(42);  // Exit with a specific status for demonstration
        
    } else {
        // Parent process
        printf("PARENT: I am the parent process (PID: %d)\n", (int) getpid());
        printf("PARENT: I created a child with PID: %d\n", rc);
        printf("PARENT: Calling wait() to wait for my child...\n");
        
        int status;
        int wait_return = wait(&status);
        
        printf("PARENT: wait() returned: %d\n", wait_return);
        printf("PARENT: This is the PID of my child that just terminated!\n");
        
        // Check if child exited normally
        if (WIFEXITED(status)) {
            printf("PARENT: Child exited normally with status: %d\n", WEXITSTATUS(status));
        }
        
        printf("PARENT: Done waiting, child has finished.\n\n");
    }
    
    printf("Process (PID: %d) is exiting.\n", (int) getpid());
    return 0;
}
