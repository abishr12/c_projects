#include <stdio.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int fd;        
    int flags;
    int newfd;
    int flags_one;
    int flags_two;

    if (argc != 2) {
        perror("problem with args");
        exit(EXIT_FAILURE);
    }
    
    // Open file
    flags = O_WRONLY | O_CREAT | O_APPEND;
    fd = open(argv[1], flags, 0644);

    // Dup file
    newfd = dup(fd);


    // write in one, check offset in the other
    write(fd, "___", 3);
    if (lseek(fd, 0, SEEK_CUR) != lseek(newfd, 0, SEEK_CUR)) {
        fprintf(stderr, "failure to match position");
        exit(EXIT_FAILURE);
    }
    // changes flag in one, verifies in the other
    flags |= O_RDWR;

    fcntl(fd, F_SETFL, flags);
    flags_one = fcntl(fd, F_GETFL);
    flags_two = fcntl(newfd, F_GETFL);
    if (flags_one != flags_two) {
        fprintf(stderr, "failure to match flags");
        exit(EXIT_FAILURE);
    }
    return 0;
}
