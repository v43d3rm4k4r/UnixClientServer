#include <iostream>
#include <fstream>
#include <csignal>
#include <cstring>
#include "../Server/TCPServer.h"

/* DAEMON TCP SERVER EXAMPLE
 * Receives text file from client and saves it.
 * Handles SIGTERM & SIGHUP signals.
*/

#define MAX_FILE_SIZE 1024U // Ethernet frame
//==================================================================================
int32_t client_fd;
TCPServer TCP_server;
void handle_signal(int32_t);
//==================================================================================
int main()
{
    // signals handling
    struct sigaction sig_handler;
    sig_handler.sa_handler = handle_signal;
    sigaction(SIGTERM, &sig_handler, nullptr);
    sigaction(SIGHUP,  &sig_handler, nullptr);

    TCP_server.listen();


    client_fd = TCP_server.accept();

    std::ofstream fout;
    char buffer[sizeof(MetaData_Upload)] = {0};

    size_t bytes_read = 0;
    while (bytes_read != sizeof(MetaData_Upload))
    {
        bytes_read += TCP_server.read(client_fd, buffer, MAX_FILE_SIZE);
    }

    auto* meta_data = reinterpret_cast<MetaData*>(buffer);
    size_t file_size = 0;
    if (meta_data->type == CmdType::Upload)
    {
        auto meta_data_upload = reinterpret_cast<MetaData_Upload*>(buffer);
        file_size = meta_data_upload->size;
        fout.open("received_"+std::string(meta_data_upload->name), std::ios_base::out);

        if (!fout.is_open())
        {
            std::cerr << "Fatal: cannot create/change file." << std::endl;
            TCP_server.close();
            close(client_fd);
            exit(EXIT_FAILURE);
        }

        // Answering with CmdType::Ready package
        MetaData answer = {};
        answer.type = CmdType::Ready;
        auto ptr = reinterpret_cast<const char*>(&answer);
        TCP_server.write(client_fd, ptr, sizeof(MetaData));

        // Reading data
        char* data = new char[file_size];
        memset(data, 0 , file_size);
        bytes_read = 0;
        while (bytes_read != file_size)
        {
            bytes_read += TCP_server.read(client_fd, data, file_size);
        }

        fout << data;

        delete[] data;
        fout.close();
    }
    return 0;
}
//==================================================================================
void handle_signal(int32_t)
{
    close(client_fd);
    TCP_server.close();
    exit(EXIT_SUCCESS);
}
