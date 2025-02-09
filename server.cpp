#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

static void msg(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg)
{
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

const size_t k_max_msg = 4096;

static int32_t read_full(int fd, char *buf, size_t n)
{
    while (n > 0)
    {
        ssize_t rv = read(fd, buf, n);
        if (rv <= 0)
        {
            return -1; // error, or unexpected EOF
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

static int32_t write_all(int fd, const char *buf, size_t n)
{
    while (n > 0)
    {
        ssize_t rv = write(fd, buf, n);
        if (rv <= 0)
        {
            return -1; // error
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

static int32_t one_request(int connfd)
{
    // 4 bytes header
    char rbuf[4 + k_max_msg];
    errno = 0;
    int32_t err = read_full(connfd, rbuf, 4);
    if (err)
    {
        msg(errno == 0 ? "EOF" : "read() error");
        return err;
    }

    uint32_t len = 0;
    memcpy(&len, rbuf, 4); // assume little endian
    if (len > k_max_msg)
    {
        msg("too long");
        return -1;
    }

    // request body
    err = read_full(connfd, &rbuf[4], len);
    if (err)
    {
        msg("read() error");
        return err;
    }

    // do something
    fprintf(stderr, "client says: %.*s\n", len, &rbuf[4]);

    // reply using the same protocol
    const char reply[] = "world";
    char wbuf[4 + sizeof(reply)];
    len = (uint32_t)strlen(reply);
    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], reply, len);
    return write_all(connfd, wbuf, 4 + len);
}

int main()
{
    // socket handle for a IPV4 TCP connection
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        die("socket()");
    }

    // setup socket options to reuse the address in time_wait state
    int val = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0)
    {
        die("setsockopt()");
    }

    // bind to an address
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);     // port
    addr.sin_addr.s_addr = htonl(0); // wildcard IP 0.0.0.0

    if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        die("bind()");
    }

    // listen for connections
    if (listen(fd, SOMAXCONN) < 0)
    {
        die("listen()");
    }

    // accept connections
    while (1)
    {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0)
        {
            continue; // error
        }

        while (1)
        {
            // here the server only serves one client connection at once
            int32_t err = one_request(connfd);
            if (err)
            {
                break;
            }
        }
        close(connfd);
    }
    return 0;
}