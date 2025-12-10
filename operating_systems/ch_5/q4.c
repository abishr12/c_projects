#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * This program demonstrates all variants of the exec() family of functions.
 * 
 * WHY SO MANY VARIANTS?
 * =====================
 * The different exec() variants provide flexibility for different use cases:
 * 
 * 1. Argument passing style:
 *    - 'l' (list): Arguments passed as separate parameters: execl(path, arg0, arg1, ..., NULL)
 *    - 'v' (vector): Arguments passed as array: execv(path, argv[])
 * 
 * 2. PATH searching:
 *    - 'p': Searches PATH environment variable for the executable
 *           (so you can use "ls" instead of "/bin/ls")
 *    - no 'p': Requires full path to executable
 * 
 * 3. Environment customization:
 *    - 'e': Allows passing custom environment variables
 *    - no 'e': Uses current process's environment
 * 
 * This gives programmers flexibility to choose the most convenient variant
 * for their specific needs without writing extra wrapper code.
 */

void demo_execl() {
    printf("\n=== Demonstrating execl() ===\n");
    printf("execl: Takes arguments as a LIST, requires full PATH\n");
    
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child executing: execl(\"/bin/ls\", \"ls\", \"-l\", NULL)\n\n");
        // Arguments: full path, program name (by convention), additional args, NULL terminator
        execl("/bin/ls", "ls", "-l", NULL);
        
        // If exec succeeds, this line never executes
        fprintf(stderr, "execl failed\n");
        exit(1);
    } else {
        // Parent waits for child
        wait(NULL);
        printf("\n[Parent] execl() demo completed\n");
    }
}

void demo_execlp() {
    printf("\n=== Demonstrating execlp() ===\n");
    printf("execlp: Takes arguments as a LIST, searches PATH\n");
    
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child executing: execlp(\"ls\", \"ls\", \"-la\", NULL)\n\n");
        // No need for full path - it searches PATH
        execlp("ls", "ls", "-la", NULL);
        
        fprintf(stderr, "execlp failed\n");
        exit(1);
    } else {
        wait(NULL);
        printf("\n[Parent] execlp() demo completed\n");
    }
}

void demo_execle() {
    printf("\n=== Demonstrating execle() ===\n");
    printf("execle: Takes arguments as a LIST, requires full PATH, custom ENVIRONMENT\n");
    
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Create a custom environment (just PATH in this case)
        char *envp[] = {
            "PATH=/bin:/usr/bin",
            "CUSTOM_VAR=Hello from execle!",
            NULL
        };
        
        printf("Child executing: execle(\"/bin/ls\", \"ls\", \"-h\", NULL, envp)\n");
        printf("(with custom environment)\n\n");
        
        // Last argument is the environment array
        execle("/bin/ls", "ls", "-h", NULL, envp);
        
        fprintf(stderr, "execle failed\n");
        exit(1);
    } else {
        wait(NULL);
        printf("\n[Parent] execle() demo completed\n");
    }
}

void demo_execv() {
    printf("\n=== Demonstrating execv() ===\n");
    printf("execv: Takes arguments as a VECTOR (array), requires full PATH\n");
    
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Arguments passed as an array
        char *args[] = {
            "ls",           // argv[0] - program name
            "-l",           // argv[1] - first argument
            "-t",           // argv[2] - sort by time
            NULL            // Must be NULL-terminated
        };
        
        printf("Child executing: execv(\"/bin/ls\", args)\n");
        printf("where args = {\"ls\", \"-l\", \"-t\", NULL}\n\n");
        
        execv("/bin/ls", args);
        
        fprintf(stderr, "execv failed\n");
        exit(1);
    } else {
        wait(NULL);
        printf("\n[Parent] execv() demo completed\n");
    }
}

void demo_execvp() {
    printf("\n=== Demonstrating execvp() ===\n");
    printf("execvp: Takes arguments as a VECTOR (array), searches PATH\n");
    
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        char *args[] = {
            "ls",
            "-a",           // Show hidden files
            "-1",           // One file per line
            NULL
        };
        
        printf("Child executing: execvp(\"ls\", args)\n");
        printf("where args = {\"ls\", \"-a\", \"-1\", NULL}\n\n");
        
        // Most commonly used variant - flexible and convenient
        execvp("ls", args);
        
        fprintf(stderr, "execvp failed\n");
        exit(1);
    } else {
        wait(NULL);
        printf("\n[Parent] execvp() demo completed\n");
    }
}

void demo_execvP() {
    printf("\n=== Demonstrating execvP() ===\n");
    printf("execvP: Like execvp, but allows specifying a custom search path\n");
    printf("NOTE: execvP() is NOT part of POSIX and may not be available on all systems\n");
    
    #ifdef __APPLE__
    // execvP is available on macOS/BSD but not Linux
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        char *args[] = {
            "ls",
            "-C",           // Multi-column output
            NULL
        };
        
        printf("Child executing: execvP(\"ls\", \"/bin:/usr/bin\", args)\n\n");
        
        // Third parameter specifies the search path
        execvP("ls", "/bin:/usr/bin", args);
        
        fprintf(stderr, "execvP failed\n");
        exit(1);
    } else {
        wait(NULL);
        printf("\n[Parent] execvP() demo completed\n");
    }
    #else
    printf("execvP() is not available on this system (requires BSD/macOS)\n");
    printf("This variant allows you to specify a custom PATH to search,\n");
    printf("which can be useful for security or custom execution environments.\n");
    #endif
}

int main(int argc, char *argv[]) {
    printf("========================================\n");
    printf("EXEC() FAMILY DEMONSTRATION\n");
    printf("========================================\n");
    printf("\nThis program demonstrates the 6 variants of exec().\n");
    printf("Each variant replaces the current process with /bin/ls\n");
    printf("and executes it with different argument styles.\n");
    
    // Demonstrate each variant
    demo_execl();
    sleep(1);  // Small pause for readability
    
    demo_execlp();
    sleep(1);
    
    demo_execle();
    sleep(1);
    
    demo_execv();
    sleep(1);
    
    demo_execvp();
    sleep(1);
    
    demo_execvP();
    
    printf("\n========================================\n");
    printf("SUMMARY: WHY SO MANY VARIANTS?\n");
    printf("========================================\n");
    printf("\nThe exec() family has multiple variants to provide flexibility:\n\n");
    
    printf("1. ARGUMENT STYLE:\n");
    printf("   - 'l' functions: Pass args as separate parameters (convenient for fixed args)\n");
    printf("   - 'v' functions: Pass args as array (convenient when args are dynamic)\n\n");
    
    printf("2. PATH SEARCHING:\n");
    printf("   - Functions with 'p': Search PATH for executable (convenient)\n");
    printf("   - Functions without 'p': Require full path (more secure/explicit)\n\n");
    
    printf("3. ENVIRONMENT:\n");
    printf("   - Functions with 'e': Can pass custom environment (for isolation)\n");
    printf("   - Functions without 'e': Inherit parent's environment (typical case)\n\n");
    
    printf("This design follows Unix philosophy: provide simple, composable tools\n");
    printf("that can be combined in different ways for different needs.\n");
    printf("Instead of one complex function with many flags, you get 6 specialized\n");
    printf("functions that are each optimal for specific use cases.\n\n");
    
    return 0;
}
