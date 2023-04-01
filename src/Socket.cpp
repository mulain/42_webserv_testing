
#include "../incl/Socket.hpp"

// Constructors and destructors
Socket::Socket(void)
{
	create(SOCK_STREAM);
}

Socket::Socket(int type)
{
	create(type);
}

Socket::~Socket(void)
{
	close();
}

Socket::Socket(const Socket& src)
{
	*this = src;
}

// Operator overloads
Socket& Socket::operator=(const Socket& src)
{
	_socketfd = src._socketfd;
	_address = src._address;
	return *this;
}

// Public member functions
bool Socket::create(int type)
{
	_socketfd = socket(AF_INET, type, 0);
	if (_socketfd == -1)
		return perror(E_SOCK_CREATE), false;
	return true;
}

bool Socket::bind(const std::string& ipAddress, int port)
{
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	_address.sin_port = htons(port);
	if (::bind(_socketfd, (sockaddr*)&_address, sizeof(_address)) == -1)
		return perror(E_SOCK_BIND), false;
	return true;
}

bool Socket::bind(in_addr_t address, uint16_t port)
{
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = address;
	_address.sin_port = port;
	if (::bind(_socketfd, (sockaddr*)&_address, sizeof(_address)) == -1)
		return perror(E_SOCK_BIND), false;
	return true;
}

bool Socket::listen(int backlog)
{
	if (::listen(_socketfd, backlog) == -1)
		return perror(E_SOCK_LISTEN), false;
	return true;
}

bool Socket::accept(Socket& newSocket)
{
	socklen_t		len = sizeof(_address);

	newSocket._socketfd = ::accept(_socketfd, (sockaddr*)&_address, &len);
	if (newSocket._socketfd == -1)
		return perror(E_SOCK_ACCEPT), false;
	return true;
}

bool Socket::connect(const std::string& ipAddress, int port)
{
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	_address.sin_port = htons(port);
	if (::connect(_socketfd, (sockaddr*)&_address, sizeof(_address)) == -1)
		return perror(E_SOCK_CONNECT), false;
	return true;
}

bool Socket::send(const std::string& data)
{
	if (::send(_socketfd, data.c_str(), data.size(), 0) == -1)
		return perror(E_SOCK_SEND), false;
	return true;
}

bool Socket::receive(std::string& data)
{
	char	buffer[BUFFERSIZE];
	int		readlen;

	readlen = ::recv(_socketfd, buffer, BUFFERSIZE, 0);
	if (readlen == -1)
		return perror(E_SOCK_RECEIVE), false;
	data = std::string(buffer, readlen);
	return true;
}

void Socket::close()
{
	if (_socketfd != -1)
		::close(_socketfd);
	_socketfd = -1;
}
