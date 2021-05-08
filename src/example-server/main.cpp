#include <iostream>
#include <fstream>
#include <csignal>
#include <cstring>
#include "../Server/TCPServer.h"

/* DAEMON TCP SERVER EXAMPLE
 * Receives text file from client and saves it.
 * Handles SIGTERM & SIGHUP signals.
*/

#define DEFAULT_RECEIVED_FILE_NAME "received_text.txt"
#define MAX_FILE_SIZE 1024u // Ethernet frame

void handle_signal(int32_t);
//==================================================================================
int main()
{
    // signals handling
    struct sigaction sig_handler;
    sig_handler.sa_handler = handle_signal;
    sigaction(SIGTERM, &sig_handler, nullptr);
    sigaction(SIGHUP,  &sig_handler, nullptr);

    TCPServer TCP_server;
    TCP_server.listen();


    int32_t client_fd = TCP_server.accept();

    char buffer[MAX_FILE_SIZE] = {0};
    ssize_t bytes_read;

    std::ofstream fout(DEFAULT_RECEIVED_FILE_NAME, std::ios_base::out);
    if (fout.is_open())
    {
        std::cerr << "Fatal: cannot create/change file " << DEFAULT_RECEIVED_FILE_NAME
                  << '.' << std::endl;
        TCP_server.close();
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = TCP_server.read(client_fd, buffer, MAX_FILE_SIZE)) != 0)
    {
        fout.write(buffer, bytes_read);
    }


    fout.close();
    return 0;
}
//==================================================================================
void handle_signal(int32_t)
{
    exit(EXIT_SUCCESS);
}
