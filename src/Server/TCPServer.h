#pragma once

#include <iostream>

#include <unistd.h> // read(), write(), close()

#include <sys/types.h>  // socket constants (AF_INET, SOCK_STREAM)
#include <sys/socket.h> // socket(), bind()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // htons()

#include <cstdlib>
#include <cstdint>

#define DEFAULT_PORT    34543
#define DEFAULT_BACKLOG 5     // maximum queue length

class TCPServer
{
    /*static*/int32_t _server_sockfd;
    /*static*/struct sockaddr_in _addr;
    bool _show_extra_info;
    bool _echo_mode;

    int32_t socket(int32_t domain, int32_t sock_type, int32_t protocol); // perror() + exit(1)
    void bind();                                                         // perror() + exit(1) || (struct sockaddr*)

public:
    TCPServer(int32_t port = DEFAULT_PORT);
    TCPServer           (const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;
    ~TCPServer();

    void setShowExtraInfo(bool flag);
    bool getShowExtraInfo() const;

    void setEchoMode(bool flag);
    bool getEchoMode() const;

    // shoud be in ctor or in setup() method
    /*
    socket
    _adr.sin_family = AF_NET;
    _adr.sin_port = htons(${port});
    bind
    */

    void    listen(int32_t backlog = DEFAULT_BACKLOG);                              // perror() + exit(1)
    int32_t accept();                                             // perror() + exit(1) || (struct sockaddr*)
    
    ssize_t read  (void* buf, size_t buf_size);                   // perror() + exit(1)
    ssize_t write (int32_t fd, const void* buf, size_t buf_size);
    void    close (int32_t fd);                                   // perror() + exit(1)
};