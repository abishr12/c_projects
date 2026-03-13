#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd;
    int num_bytes;
    int flags;
    int third_arg = 0;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s pathname \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 4) {
        third_arg = 1;
    }

    flags = O_CREAT | O_WRONLY | O_APPEND;

    if (third_arg) {
        flags &= ~O_APPEND;
    }

    fd = open(argv[1], flags, 0644);

    if (fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }

    num_bytes = atoi(argv[2]);

    for (int i = 0; i < num_bytes; i++) {
        if (third_arg && lseek(fd, 0, SEEK_END) == -1){
            perror("lseek");
            exit(EXIT_FAILURE);
        }
        write(fd, "-", 1);
    }

    exit(EXIT_SUCCESS);
}
