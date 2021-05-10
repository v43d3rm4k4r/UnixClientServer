#include <iostream>
#include <fstream>
#include <csignal>
#include <cstring>
#include "../Server/TCPServer.h"

/* DAEMON TCP SERVER EXAMPLE
 * Receives text file from client and saves it.
 * Handles SIGTERM & SIGHUP signals.
*/

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

    std::ofstream fout;
    char buffer[sizeof(MetaData_Download)] = {0};
    size_t bytes_read;
    // Reading meta data buffer
    while ((bytes_read = TCP_server.read(client_fd, buffer, MAX_FILE_SIZE)) != 0)
    {
    }
    auto* meta_data = reinterpret_cast<MetaData*>(buffer);
    size_t file_size = 0;
    if (meta_data->type == CmdType::Upload)
    {
        auto meta_data_upload = reinterpret_cast<MetaData_Upload*>(buffer);
        file_size = meta_data_upload->size;
        fout.open("received_"+std::string(meta_data_upload->name), std::ios_base::out);
        if (fout.is_open())
        {
            std::cerr << "Fatal: cannot create/change file." << std::endl;
            TCP_server.close();
            close(client_fd);
            exit(EXIT_FAILURE);
        }

        // answering with CmdType::Ready package
        char* answer = new char[sizeof(MetaData)];
        memset(answer, 0, sizeof(MetaData));
        MetaData* ptr = reinterpret_cast<MetaData*>(answer);
        ptr->type = CmdType::Ready;
        TCP_server.write(client_fd, answer, sizeof(answer));
        delete[] answer;
    }

    // Reading data
    char* data = new char[MAX_FILE_SIZE];
    memset(data, 0 , MAX_FILE_SIZE);
    while ((bytes_read = TCP_server.read(client_fd, data, MAX_FILE_SIZE)) != file_size ||
           bytes_read != 0)
    {
        fout.write(data, bytes_read);
    }
    delete[] data;

    fout.close();
    return 0;
}
//==================================================================================
void handle_signal(int32_t)
{
    exit(EXIT_SUCCESS);
}
