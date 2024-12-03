/*
 * linux.c
 *
 * Name: openhttp
 * Description: Open-source HTTP server written in pure C
 * Version: 1.0.0
 *
 * Author: Kevin Alavik <kevin@alavik.se>
 * Year: 2024
 *
 * This file contains linux-specific code for the OpenHTTP server.
 *
 * Contributors:
 *  - Kevin Alavik <kevin@alavik.se>
 */

#include <openhttp.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <openhttp.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#ifdef __linux__

#define MAX_EVENTS 64

static __thread int _linux_epoll_fd = -1;
static __thread int _linux_listen_fd = -1;

static __thread int _linux_client_sockets[MAX_EVENTS] = {0};
static __thread int _linux_client_count = 0;

int _openhttp_linux_server_spawn(int _port, _openhttp_client_handler_t client_handler)
{
    int listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (listen_fd == -1)
    {
        _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "Failed to create server socket");
        return OPENHTTP_UNKNOWN_ERROR;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(_port);

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "Failed to bind server socket");
        close(listen_fd);
        return OPENHTTP_UNKNOWN_ERROR;
    }

    if (listen(listen_fd, SOMAXCONN) == -1)
    {
        _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "Failed to listen on server socket");
        close(listen_fd);
        return OPENHTTP_UNKNOWN_ERROR;
    }

    _linux_listen_fd = listen_fd;

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "Failed to create epoll instance");
        close(listen_fd);
        return OPENHTTP_UNKNOWN_ERROR;
    }

    _linux_epoll_fd = epoll_fd;

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1)
    {
        _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "Failed to add listen socket to epoll instance");
        close(listen_fd);
        close(epoll_fd);
        return OPENHTTP_UNKNOWN_ERROR;
    }

    struct epoll_event events[MAX_EVENTS];

    while (1)
    {
        int nfd = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfd == -1)
        {
            _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "epoll_wait failed");
            break;
        }

        for (int i = 0; i < nfd; i++)
        {
            if (events[i].data.fd == listen_fd)
            {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_fd == -1)
                {
                    _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "Failed to accept client connection");
                    continue;
                }

                fcntl(client_fd, F_SETFL, O_NONBLOCK);

                if (_linux_client_count < MAX_EVENTS)
                {
                    _linux_client_sockets[_linux_client_count++] = client_fd;
                }

                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
                {
                    _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "Failed to add client socket to epoll instance");
                    close(client_fd);
                    continue;
                }
            }
            else if (events[i].events & EPOLLIN)
            {
                int client_fd = events[i].data.fd;
                char buffer[1024];
                ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
                if (bytes_read == -1)
                {
                    if (errno != EAGAIN)
                    {
                        _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "Failed to read from client socket");
                        close(client_fd);
                    }
                }
                else if (bytes_read == 0)
                {
                    close(client_fd);
                }
                else
                {
                    buffer[bytes_read] = '\0';
                    client_handler(client_fd, buffer);
                    close(client_fd);
                }
            }
        }
    }

    close(listen_fd);
    close(epoll_fd);
    return OPENHTTP_SUCCESS;
}

int _openhttp_linux_cleanup()
{
    for (int i = 0; i < _linux_client_count; i++)
    {
        close(_linux_client_sockets[i]);
    }
    _linux_client_count = 0;

    if (_linux_epoll_fd != -1)
    {
        close(_linux_epoll_fd);
        _linux_epoll_fd = -1;
    }

    if (_linux_listen_fd != -1)
    {
        close(_linux_listen_fd);
        _linux_listen_fd = -1;
    }

    return OPENHTTP_SUCCESS;
}

int _openhttp_linux_server_callback(int client_fd, const char *request)
{
    printf("Received request from client %d: %s\n", client_fd, request);

    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    if (write(client_fd, response, strlen(response)) == -1)
    {
        _openhttp_raise_error(OPENHTTP_SYSTEM_ERROR, "Failed to write response to client socket");
        return OPENHTTP_SYSTEM_ERROR;
    }

    return OPENHTTP_SUCCESS;
}

#endif // __linux__

/*
 * License: MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * provided to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
