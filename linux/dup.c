#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int dup(int oldfd) {
    int newfd;

    newfd = fcntl(oldfd, F_DUPFD, 0);

    return newfd;
}

int dup2(int oldfd, int newfd){
    if (oldfd == newfd) {
        if (fcntl(oldfd, F_GETFL) == -1) {
            errno = EBADF;
            return -1;
        }
        return oldfd;
    }

    if(fcntl(oldfd, F_GETFL) == -1){
        errno = EBADF;
        return -1;
    }
    if(fcntl(newfd, F_GETFL) != -1){
        close(newfd);
    }
    if(fcntl(oldfd, F_DUPFD, newfd) == -1){
        return -1;
    }

    return newfd;
}
