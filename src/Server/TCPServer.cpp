#include "TCPServer.h"

TCPServer::TCPServer(int32_t port/*=34543*/)
	: _server_sockfd{0},
    _addr{ 0, 0, {0}, {0} },
	_show_extra_info{true},
	_echo_mode{true}
{
    _server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    _addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
    //_addr.sin_addr.s_addr = htonl(INADDR_ANY); // not needed while _addr{0}
    bind();
}
//==================================================================================
int32_t TCPServer::socket(int32_t domain, int32_t sock_type, int32_t protocol)
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
void TCPServer::bind()
{
    if (::bind(_server_sockfd, reinterpret_cast<struct sockaddr*>(&_addr), sizeof(_addr)) == -1)
	{
        print(" error: ", __PRETTY_FUNCTION__);
        perror(nullptr);
        close();
		exit(EXIT_FAILURE);
	}
}
//==================================================================================
void TCPServer::listen(int32_t backlog/*=5*/)									
{
	if (_show_extra_info && backlog > 5)
	{
        print(" warning: backlog is more than 5", __PRETTY_FUNCTION__);
	}
	if (::listen(_server_sockfd, backlog) == -1)
	{
        print(" error: ", __PRETTY_FUNCTION__);
        perror(nullptr);
        close();
		exit(EXIT_FAILURE);
	}
}
//==================================================================================
int32_t TCPServer::accept()
{
    socklen_t addr_len = sizeof _addr;
    int32_t ret = ::accept(_server_sockfd, reinterpret_cast<struct sockaddr*>(&_addr), &addr_len);
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
ssize_t TCPServer::read(int32_t fd, void* buf, size_t buf_size)
{
    ssize_t bytes_readed = ::read(fd, buf, buf_size);
	if (bytes_readed == -1)
	{
        print(" error: ", __PRETTY_FUNCTION__);
        perror(nullptr);
        close();
		exit(EXIT_FAILURE);
	}
	else if (_show_extra_info && bytes_readed == 0)
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
ssize_t TCPServer::write(int32_t fd, const void* buf, size_t buf_size)
{
	ssize_t bytes_wrote = ::write(fd, buf, buf_size);
	if (bytes_wrote == -1)
	{
        print(" error: ", __PRETTY_FUNCTION__);
        perror(nullptr);
        close();
		exit(EXIT_FAILURE);
	}
	else if (_show_extra_info)
	{
        print("wrote " + std::to_string(bytes_wrote) + " bytes",
              __PRETTY_FUNCTION__);
	}
	return bytes_wrote;
}
//==================================================================================
int32_t TCPServer::getSocket() const
{
	return _server_sockfd;
}
//==================================================================================
void TCPServer::setShowExtraInfo(bool flag)
{
	_show_extra_info = flag;
}
//==================================================================================
bool TCPServer::getShowExtraInfo() const
{
	return _show_extra_info;
}
//==================================================================================
void TCPServer::setEchoMode(bool flag)
{
	_echo_mode = flag;
}
//==================================================================================
bool TCPServer::getEchoMode() const
{
	return _echo_mode;
}
//==================================================================================
void TCPServer::print(std::string str, const char* func)
{
    std::cout << "[SERVER] " << func << ": ";

    if (!str.empty())
        std::cout << str;

    //std::cout << std::endl;
}
//==================================================================================
void TCPServer::close()
{
    ::close(_server_sockfd);
}
