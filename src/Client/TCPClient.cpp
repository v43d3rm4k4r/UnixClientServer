#include "TCPServer.h"

TCPClient::TCPClient(int32_t port/*=34543*/)
	: _client_sockfd{0},
	_addr{0},
	_show_extra_info{true}
{
	_client_sockfd = socket(AF_NET, SOCK_STREAM, 0);

	//bind();
	_addr.sin_family = AF_NET;
	_addr.sin_port = htons(port);
	//_addr.sin_addr.s_addr = htonl(INADDR_ANY); // not needed while _addr{0}
	inet_pton(AF_NET, (struct sockaddr*)&_addr, sizeof(_addr)); // if local server
	connect(_client_sockfd, (struct sockaddr*)&_addr, sizeof(_addr));
}
//==================================================================================
int32_t TCPClient::socket(int32_t domain, int32_t sock_type, int32_t protocol)
{
	int32_t ret = ::socket(domain, type, protocol);
	if (ret == -1)
	{
		perror("[CLIENT] socket() error");
		exit(EXIT_FAILURE);
	}
	return ret;
}
//==================================================================================
void TCPClient::inet_pton(int32_t afamily, const char* src, void* dst)
{
	int32_t ret = ::inet_pton(afamily, src, dst);
	if (ret == 0)
	{
		// no errno value
		std::cout << "[CLIENT] inet_pton() error: src does not contain a character"
		" string representing a valid network address in the specified address family" 
		<< std::endl;
		exit(EXIT_FAILURE);
	}
	else if (ret == -1)
	{
		perror("[CLIENT] inet_pton() error");
		exit(EXIT_FAILURE);
	}
}
//==================================================================================
void TCPClient::connect()
{
	if (::connect(_client_sockfd, _addr, sizeof(_addr) == -1)
	{
		perror("[CLIENT] connect() error");
		exit(EXIT_FAILURE);
	}
}
//==================================================================================
ssize_t TCPClient::read(int32_t fd, void* buf, size_t buf_size)
{
	ssize_t bytes_readed = ::read(fd, buf, buf_size);
	if (bytes_readed == -1)
	{
		perror("[CLIENT] read() error");
		exit(EXIT_FAILURE);
	}
	else if (_show_extra_info && bytes_readed == 0)
	{
		std::cout << "[CLIENT] reading: end of file reached" << std::endl;
	}
	if (_echo_mode)
	{
		std::cout << "[ECHO CLIENT] \"";
		::write(STDOUT_FILENO, buf, bytes_readed); // using outer write() due to avoid writing extra info
		std::cout << "\"" << std::endl;
	}
	return bytes_readed;
}
//==================================================================================
ssize_t TCPClient::write(int32_t fd, const void* buf, size_t buf_size)
{
	ssize_t bytes_wrote = ::write(fd, buf, buf_size);
	if (bytes_wrote == -1)
	{
		perror("[CLIENT] write() error");
		exit(EXIT_FAILURE);
	}
	else if (_show_extra_info)
	{
		std::cout << "[CLIENT] writing: wrote " << bytes_wrote << " bytes" << std::endl;
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
bool TCPClient::setEchoMode() const
{
	return _echo_mode;
}
