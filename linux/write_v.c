#include <sys/uio.h>
#include <stdlib.h>
#include <unistd.h>

ssize_t writev(int fd, const struct iovec *iov, int iovcnt){
    int i;
    char *buffer;
    size_t mem = 0;
    int final; 

    
    // Gather memory length
    for (i = 0; i < iovcnt; i++){
        mem += iov[i].iov_len;
    }
    // Create buffer
    buffer = malloc(mem);
    char *ptr = buffer;

    // Write to buffer
    for (i = 0; i < iovcnt; i++){
        memcpy(ptr, iov[i].iov_base, iov[i].iov_len);
        ptr += iov[i].iov_len;
    }

    // Final write
    final = write(fd,buffer, mem);
    free(buffer);
    return final;
}

ssize_t readv(int fd, const struct iovec *iov, int iovcnt){
    char *buffer;
    ssize_t bytes_read;
    int i;
    size_t mem = 0;

    // Gather memory length
    for (i = 0; i < iovcnt; i++){
        mem += iov[i].iov_len;
    }

    // Allocate memory to buffer
    buffer = malloc(mem);

    // write file to buffer
    bytes_read = read(fd, buffer, mem);

    // split data between iov buffers
    
    char *ptr = buffer;

    // Write to iov buffers
    for (i = 0; i < iovcnt; i++){
        memcpy(iov[i].iov_base, ptr, iov[i].iov_len);
        ptr += iov[i].iov_len;
    }

    free(buffer);
    return bytes_read;
}

