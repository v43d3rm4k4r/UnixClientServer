#pragma once

#include <iostream>

#include <unistd.h>     // read(), write(), close()

#include <sys/types.h>  // socket constants (AF_INET, SOCK_STREAM)
#include <sys/socket.h> // socket(), bind()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // htons()

#include <cstdlib>
#include <cstdint>

#define DEFAULT_PORT    34543
#define DEFAULT_BACKLOG 5     // maximum queue length

// Note: сlosing server`s and accepted sockets is after the client application.

class TCPServer
{
    int32_t _server_sockfd;
    struct sockaddr_in _addr;
    bool _show_extra_info;
    bool _echo_mode;

    int32_t socket(int32_t domain, int32_t sock_type, int32_t protocol);
    void bind();                                                         

public:
    TCPServer(int32_t port = DEFAULT_PORT);
    TCPServer           (const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;
    ~TCPServer();

    int32_t getSocket() const;

    void setShowExtraInfo(bool flag);
    bool getShowExtraInfo() const;

    void setEchoMode(bool flag);
    bool getEchoMode() const;

    void    listen(int32_t backlog = DEFAULT_BACKLOG);
    int32_t accept();
    ssize_t read  (void* buf, size_t buf_size);
    ssize_t write (int32_t fd, const void* buf, size_t buf_size);
};