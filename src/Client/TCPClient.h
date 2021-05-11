#pragma once

#include <iostream>

#include <unistd.h>     // read(), write(), close()

#include <sys/types.h>  // socket constants (AF_INET, SOCK_STREAM)
#include <sys/socket.h> // socket(), bind()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // htons(), inet_pton()

#include <cstdlib>
#include <cstdint>

#include "../Protocol/Protocol.h" // used in example program

#define LOCALHOST       "127.0.0.1"
#define DEFAULT_PORT    34543

// Note: сlosing client`s socket is after the client application.

class TCPClient
{
    int32_t _client_sockfd;
    struct sockaddr_in _addr;
    bool _show_extra_info;
    bool _echo_mode;

    void print(std::string str, const char* func = nullptr);
    int32_t socket(int32_t domain, int32_t sock_type, int32_t protocol);
    void connect();
    void inet_pton(const char* ip);

public:
    TCPClient(const char* ip = LOCALHOST, int32_t port = DEFAULT_PORT);
    TCPClient           (const TCPClient&) = delete;
    TCPClient& operator=(const TCPClient&) = delete;

    int32_t getSocket() const;

    void setShowExtraInfo(bool flag);
    bool getShowExtraInfo() const;

    void setEchoMode(bool flag);
    bool getEchoMode() const;

    ssize_t read  (void* buf, size_t buf_size);
    ssize_t write (const void* buf, size_t buf_size);
    void    close ();
};
