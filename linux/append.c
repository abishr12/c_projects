#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd;
    off_t off;

    if (argc != 2 ) {
        fprintf(stderr, "Usage: %s pathname \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_WRONLY | O_APPEND);
    if (fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (lseek(fd, 0, SEEK_SET)== -1) {
        perror("lseek");
        exit(EXIT_FAILURE);
    }
            
    if (write(fd, "test", 4) == -1) {
        
        perror("write");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
