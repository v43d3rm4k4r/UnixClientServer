#include "TCPClient.h"

TCPClient::TCPClient(const char* ip/*=LOCALHOST*/, int32_t port/*=34543*/)
	: _client_sockfd{0},
    _addr{ 0, 0, {0}, {0} },
	_show_extra_info{true}
{
    _client_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//bind();
    _addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	//_addr.sin_addr.s_addr = htonl(INADDR_ANY); // not needed while _addr{0}
    inet_pton(ip);
    connect();
}
//==================================================================================
int32_t TCPClient::socket(int32_t domain, int32_t sock_type, int32_t protocol)
{
    int32_t ret = ::socket(domain, sock_type, protocol);
	if (ret == -1)
	{
        print(" error: ", __PRETTY_FUNCTION__);
        perror(nullptr);
        close();
		exit(EXIT_FAILURE);
	}
	return ret;
}
//==================================================================================
void TCPClient::inet_pton(const char* ip)
{
    int32_t ret = ::inet_pton(AF_INET, ip, &_addr.sin_addr);
	if (ret == 0)
	{
		// no errno value
        print("error: src does not contain a character"
        " string representing a valid network address in the specified address family");
        close();
		exit(EXIT_FAILURE);
	}
	else if (ret == -1)
	{
        print(" error: ", __PRETTY_FUNCTION__);
        perror(nullptr);
        close();
		exit(EXIT_FAILURE);
	}
}
//==================================================================================
void TCPClient::connect()
{
    if (::connect(_client_sockfd, reinterpret_cast<struct sockaddr*>(&_addr),
                  sizeof(_addr)) == -1)
	{
        print(" error: ", __PRETTY_FUNCTION__);
        perror(nullptr);
        close();
		exit(EXIT_FAILURE);
	}
}
//==================================================================================
ssize_t TCPClient::read(void* buf, size_t buf_size)
{
    ssize_t bytes_readed = ::read(_client_sockfd, buf, buf_size);
	if (bytes_readed == -1)
	{
        print(" error: ", __PRETTY_FUNCTION__);
        perror(nullptr);
        close();
		exit(EXIT_FAILURE);
	}
    if (_show_extra_info)
    {
        print("read " + std::to_string(bytes_readed) + " bytes",
              __PRETTY_FUNCTION__);
    }
    if (_show_extra_info && bytes_readed == 0)
    {
        print("end of file reached");
    }
	if (_echo_mode)
	{
        print("echo message: ");
		::write(STDOUT_FILENO, buf, bytes_readed); // using outer write() due to avoid writing extra info
	}
	return bytes_readed;
}
//==================================================================================
ssize_t TCPClient::write(const void* buf, size_t buf_size)
{
    ssize_t bytes_wrote = ::write(_client_sockfd, buf, buf_size);
	if (bytes_wrote == -1)
	{
        print(" error: ", __PRETTY_FUNCTION__);
        perror(nullptr);
        close();
		exit(EXIT_FAILURE);
	}
    if (_show_extra_info)
    {
        print("wrote " + std::to_string(bytes_wrote) + " bytes",
              __PRETTY_FUNCTION__);
    }
	return bytes_wrote;
}
//==================================================================================
int32_t TCPClient::getSocket() const
{
	return _client_sockfd;
}
//==================================================================================
void TCPClient::setShowExtraInfo(bool flag)
{
	_show_extra_info = flag;
}
//==================================================================================
bool TCPClient::getShowExtraInfo() const
{
	return _show_extra_info;
}
//==================================================================================
void TCPClient::setEchoMode(bool flag)
{
	_echo_mode = flag;
}
//==================================================================================
bool TCPClient::getEchoMode() const
{
	return _echo_mode;
}
//==================================================================================
void TCPClient::print(std::string str, const char* func)
{
    std::cout << "[CLIENT] " << func << ": ";

    if (!str.empty())
        std::cout << str;

    std::cout << std::endl;
}
//==================================================================================
void TCPClient::close()
{
    ::close(_client_sockfd);
}
