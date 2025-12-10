#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Set x to 100 BEFORE fork()
    int x = 100;
    printf("Initial value: x = %d\n\n", x);
    
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // CHILD PROCESS
        printf("[CHILD] Initial value of x: %d\n", x);
        
        // Modify x in child
        x = 200;
        printf("[CHILD] After modification, x: %d\n\n", x);
        
    } else {
        // PARENT PROCESS
        // Wait a tiny bit to let child print first (for cleaner output)
        sleep(1);
        
        printf("[PARENT] Initial value of x: %d\n", x);
        
        // Modify x in parent
        x = 300;
        printf("[PARENT] After modification, x: %d\n\n", x);
        
        // Wait for child to finish
        wait(NULL);
        printf("[PARENT] Child has finished. Parent's x is still: %d\n", x);
    }
    return 0;
}