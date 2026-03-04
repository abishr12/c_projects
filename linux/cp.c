#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int is_hole(char *buf, ssize_t n) {
    for (ssize_t i = 0; i < n; i++) {
        if (buf[i] != '\0')
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int src = open(argv[1], O_RDONLY);
    if (src == -1){
        exit(1);
    }
    int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst == -1){
        exit(1);
    }

    char buf[1024];
    ssize_t n;

    while ((n = read(src, buf, sizeof(buf))) > 0) {
        if (is_hole(buf, n)) {
            if (lseek(dst, n, SEEK_CUR) == -1) {
                perror("lseek");
                exit(1);
            }
        } else {
            if (write(dst, buf, n) == -1) {
                perror("write");
                exit(1);
            }
        }
    }
    write(dst, "\0", 1);
    if (n == -1){
        exit(1);
    }

    return 0;
}

