#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Open a file for writing (create if it doesn't exist, truncate if it does)
    int fd = open("./q2_output.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    
    if (fd < 0) {
        fprintf(stderr, "Failed to open file\n");
        return 1;
    }
    
    printf("File opened with file descriptor: %d\n", fd);
    
    // Fork the process
    int rc = fork();
    
    if (rc < 0) {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (rc == 0) {
        // Child process
        printf("Child process (PID: %d) attempting to write...\n", getpid());
        
        // Child writes to the file multiple times
        for (int i = 0; i < 5; i++) {
            char buffer[100];
            sprintf(buffer, "Child writing line %d\n", i);
            write(fd, buffer, strlen(buffer));
            // Small delay to simulate some work
            usleep(100);
        }
        
        printf("Child finished writing\n");
        close(fd);
    } else {
        // Parent process
        printf("Parent process (PID: %d) attempting to write...\n", getpid());
        
        // Parent writes to the file multiple times
        for (int i = 0; i < 5; i++) {
            char buffer[100];
            sprintf(buffer, "Parent writing line %d\n", i);
            write(fd, buffer, strlen(buffer));
            // Small delay to simulate some work
            usleep(100);
        }
        
        printf("Parent finished writing\n");
        
        // Wait for child to complete
        wait(NULL);
        close(fd);
        
        // Now let's read and display what was written
        printf("\n--- Contents of q2_output.txt ---\n");
        FILE *file = fopen("./q2_output.txt", "r");
        if (file) {
            char line[256];
            while (fgets(line, sizeof(line), file)) {
                printf("%s", line);
            }
            fclose(file);
        }
        printf("--- End of file ---\n");
    }
    
    return 0;
}
