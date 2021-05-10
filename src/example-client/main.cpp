#include <iostream>
#include <fstream>
#include <cstring>
#include "../Client/TCPClient.h"

/* TCP CLIENT EXAMPLE
 * Send text file to server.
*/

#define DEFAULT_FILE_NAME "text.txt"
#define MAX_FILE_SIZE 1024u // Ethernet frame

size_t getFileSize(std::ifstream& fin);

int main(int argc, char* argv[])
{
    std::ifstream fin;
    bool is_default_file = false;
    if (argc == 2 && argv[1][strlen(argv[1])-2] == 't' && argv[1][strlen(argv[1])-3] == 'x'
            && argv[1][strlen(argv[1])-4] == 't' && argv[1][strlen(argv[1])-5] == '.')
    {
        fin.open(argv[1], std::ios_base::in);
        if (!fin.is_open())
        {
            std::cout << "Cannot open " << argv[1] << " file." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fin.open(DEFAULT_FILE_NAME, std::ios_base::in);
        if (!fin.is_open())
        {
            std::cout << "Cannot open " << DEFAULT_FILE_NAME << " file." << std::endl;
            exit(EXIT_FAILURE);
        }
        is_default_file = true;
    }


    TCPClient TCP_client;

    // Uploading meta data
    char* buf = new char[sizeof(MetaData_Upload)];
    memset(buf, 0, sizeof(MetaData_Upload));
    auto* ptr = reinterpret_cast<MetaData_Upload*>(buf);

    ptr->type = CmdType::Upload;
    ptr->size = getFileSize(fin);
    strncpy(ptr->name, is_default_file ? DEFAULT_FILE_NAME : argv[1], MAX_FILE_NAME_SIZE);
    TCP_client.write(buf, sizeof(buf));
    delete[] buf;

    // Waiting for CmdType::Ready
    char* answer = new char[sizeof(MetaData)];
    memset(answer, 0, sizeof(MetaData));
    size_t bytes_read;
    // Reading meta data buffer
    while ((bytes_read = TCP_client.read(answer, sizeof(MetaData))) != 0)
    {
    }
    auto* ans = reinterpret_cast<MetaData*>(answer);
    if (ans->type == CmdType::Upload)
    {
        // Uploading the data
        buf = new char[MAX_FILE_SIZE];
        while (fin.read(buf, MAX_FILE_SIZE)) // !!
        {
            TCP_client.write(buf, MAX_FILE_SIZE); // !!
        }
    }
    delete[] answer;

    return 0;
}
//==================================================================================
size_t getFileSize(std::ifstream& fin)
{
    char ch;
    size_t size = 0;
    while (fin >> ch)
        ++size;
    return size;
}
