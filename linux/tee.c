#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    int opt;
    
    while ((opt = getopt(argc, argv, "a")) != -1) {
        switch(opt) {
            case 'a':
                flags = O_WRONLY | O_CREAT | O_APPEND;
                break;
            default:
                flags = O_WRONLY | O_CREAT | O_TRUNC;
                break;
        }
    }
	int fd = open(argv[optind], flags, 0644);

	if (fd == -1) {
		perror("open");
		exit(1);
	}

	char buf[1024];
	ssize_t n;

	while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
		ssize_t fdo = write(STDOUT_FILENO, buf, n);

		ssize_t fdt = write(fd, buf, n);

		if (fdo == -1 || fdt == -1) {
			perror("write");
			exit(1);
		}
	}
	if (n == -1) {
		perror("read");
		exit(1);
	}

	return 0;
}
