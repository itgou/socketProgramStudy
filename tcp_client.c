#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <arpa/inet.h>

#define MESSAGE_SIZE  1024000
#define SERVER_ADDR  "127.0.0.1"
#define SERVER_PORT 12345

void sendData(int connFd);


void sendData(int connFd) {
    char *query;
    query = malloc( MESSAGE_SIZE +1);

    for (int i = 0; i < MESSAGE_SIZE ; i++){
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *cp;
    cp = query;

    long remaining = (long)strlen(cp);

    while (remaining) {
        long n_written = send(connFd, cp, remaining, 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if (n_written <= 0) {
            error(1, errno, "send failed \n");
            return;
        }
        remaining -= n_written;
        cp += n_written;
    }
    return;
}

int main() {
    int fd;
    struct sockaddr_in serverAddr;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons( SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);
    int connect_rt = connect(fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    fprintf(stdout,"conn_rt: %d \n",connect_rt); //为什么这里打印出来始终为 0 呢?
    if(connect_rt<=0){
        error(1,errno,"connect failed");
    }
    sendData(connect_rt);

    exit(0);
}
