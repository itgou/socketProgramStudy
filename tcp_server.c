#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void read_data(int connFd);

size_t readn(int fd, void *buffer, size_t size);

int main() {
    int port = 12345;
    int fd, connFd;
    socklen_t clientAddrLen;
    struct sockaddr_in serverAddr, clientAddr;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    bind(fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    listen(fd, 1024);

    for (;;) {
        clientAddrLen = sizeof(clientAddr);
        connFd = accept(fd, (struct sockaddr *) &clientAddr, &clientAddrLen);
        printf("connFD:%d \n", connFd);
        read_data(connFd);
        close(connFd);
    }
    return 0;
}

void read_data(int connFd) {
    ssize_t n;
    char buf[10240];

    int time = 0;
    for (;;) {
        fprintf(stdout, "block in read \n");
        if (n = readn(connFd, (void *) buf, (size_t) 10240) == 0) {
            return;
        }
        fprintf(stdout, "1k read for %d \n", time);
        time++;
        usleep(1000);
    }

}

size_t readn(int fd, void *buffer, size_t size) {
    size_t length;
    ssize_t nread;
    char *buffer_pointer;

    length = size;
    buffer_pointer = buffer;

    while (length > 0) {

        if ((nread = read(fd, buffer_pointer, length)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            printf("in readn func: read length: %d  \n", nread );
            break;
        }
        length -= nread;
        buffer_pointer += nread;
    }
    return size - length;
}
