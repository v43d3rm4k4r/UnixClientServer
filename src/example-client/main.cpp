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
#define MAX_FILE_SIZE 1024u // Ethernet frame

size_t getFileSize(std::string& file_name);

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


    TCPClient TCP_client;

    // Uploading meta data
    /*MetaData_Upload meta_data_upload;
    memset(&meta_data_upload, 0, sizeof(meta_data_upload));
    meta_data_upload.type = CmdType::Upload;
    meta_data_upload.size = getFileSize(file_name);
    strncpy(meta_data_upload.name, file_name.c_str(), sizeof(meta_data_upload.name));
    auto buf = reinterpret_cast<const char*>(&meta_data_upload);
    TCP_client.write(buf, sizeof(buf));*/
    char* buf = new char[sizeof(MetaData_Upload)];
    memset(buf, 0, sizeof(MetaData_Upload));
    auto ptr = reinterpret_cast<MetaData_Upload*>(buf);
    ptr->type = CmdType::Upload;
    ptr->size = getFileSize(file_name);
    strncpy(ptr->name, file_name.c_str(), MAX_FILE_NAME_SIZE);
    TCP_client.write(buf, sizeof(MetaData_Upload));
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
        char* data = new char[MAX_FILE_SIZE];
        while (fin.read(data, MAX_FILE_SIZE))
        {
            TCP_client.write(data, MAX_FILE_SIZE);
        }
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
