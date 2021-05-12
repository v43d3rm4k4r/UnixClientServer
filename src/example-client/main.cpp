#include <iostream>
#include <fstream>
#include <cstring>
#include <regex>
#include <filesystem>
#include "../Client/TCPClient.h"

/* TCP CLIENT EXAMPLE
 * Send text file to server.
*/

#define DEFAULT_FILE_NAME "text.txt"
#define MAX_FILE_SIZE     1024U // Ethernet frame
//==================================================================================
size_t getFileSize(std::string& file_name);
//==================================================================================
int main(int argc, char* argv[])
{
    std::string file_name;
    if (argc == 2)
    {
        file_name = argv[1];
        std::regex re("\\.txt$");
        if (!std::regex_match(file_name, re))
        {
            std::cout << "Warning: input file is not .txt" << std::endl;
        }
    }
    else
    {
        file_name = DEFAULT_FILE_NAME;
    }
    std::ifstream fin;
    fin.open(file_name.c_str(), std::ios_base::in);
    if (!fin.is_open())
    {
        std::cout << "Cannot open " << file_name << " file." << std::endl;
        exit(EXIT_FAILURE);
    }
    size_t file_size = getFileSize(file_name);


    TCPClient TCP_client;

    // Sending query to upload
    MetaData_Upload query = {};
    query.type = CmdType::Upload;
    query.size = file_size;
    strncpy(query.name, file_name.c_str(), MAX_FILE_NAME_SIZE);
    auto ptr = reinterpret_cast<MetaData_Upload*>(&query);
    TCP_client.write(ptr, sizeof(MetaData_Upload));

    // Waiting for CmdType::Ready
    char* answer = new char[sizeof(MetaData)];
    memset(answer, 0, sizeof(MetaData));
    // Reading meta data buffer
    size_t bytes_read = 0;
    while (bytes_read != sizeof(MetaData))
    {
        bytes_read += TCP_client.read(answer, MAX_FILE_SIZE);
    }
    auto* ans = reinterpret_cast<MetaData*>(answer);
    if (ans->type == CmdType::Ready)
    {
        // Uploading the data
        char* data = new char[file_size];
        memset(data, 0, file_size);
        fin.read(data, file_size);
        if (!fin)
            std::cout << "[CLIENT] error: only " << fin.gcount() <<
                         " could be read." << std::endl;

        TCP_client.write(data, file_size);
        delete[] data;
    }
    delete[] answer;

    return 0;
}
//==================================================================================
size_t getFileSize(std::string& file_name)
{
    std::string path = std::filesystem::canonical(file_name.c_str());
    return std::filesystem::file_size(path);
}
