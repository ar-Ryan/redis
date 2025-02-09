#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to print an error message and exit the program
void die(const char *msg) {
    perror(msg);
    exit(1);
}

// Function to handle communication with the client
static void do_something(int connfd) {
    char rbuf[64] = {};
    // Read data from the client
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        die("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    // Send a response to the client
    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main() {
    // socket handle for a IPV4 TCP connection
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    // setup socket options to reuse the address in time_wait state
    int val = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
        die("setsockopt()");
    }

    // bind to an address
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);        // port
    addr.sin_addr.s_addr = htonl(0);    // wildcard IP 0.0.0.0
    
    if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
        die("bind()");
    }

    // listen for connections
    if (listen(fd, SOMAXCONN) < 0) {
        die("listen()");
    }

    // accept connections
    while (1) {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0) {
            continue;   // error
        }

        do_something(connfd);
        close(connfd);
    }


    return 0;
}