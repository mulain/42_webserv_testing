
#include "../incl/Socket.hpp"

// Constructors and destructors
Socket::Socket(void)
{}

Socket::~Socket(void)
{}

Socket::Socket(const Socket& src)
{
	*this = src;
}

// Operator overloads
Socket& Socket::operator=(const Socket& src)
{
	_sock = src._sock;
	return *this;
}

// Public member functions
bool Socket::create(int type)
{
	_sock = socket(AF_INET, type, 0);
	if (_sock == -1)
		return perror(E_SOCK_CREATE), false;
	return true;
}

bool Socket::bind(const std::string& ipAddress, int port)
{
	sockaddr_in		address;

	address.sin_family = AF_INET;
	address.sin_addr = inet_addr(ipAddress.c_str());
	address.sin_port = htons(port);
	if (::bind(_sock, &address, sizeof(address)) == -1)
		return perror(E_SOCK_BIND), false;
	return true;
}

bool Socket::listen(int backlog)
{
	if (::listen(_sock, backlog) == -1)
		return perror(E_SOCK_LISTEN), false;
	return true;
}

bool Socket::accept(Socket& newSocket)
{
	sockaddr_in		address;
	socklen_t		len = sizeof(address);

	newSocket._sock = ::accept(_sock, (sockaddr*)&address, &len);
	if (newSocket._sock == -1)
		return perror(E_SOCK_ACCEPT), false;
	return true;
}

bool Socket::connect(const std::string& ipAddress, int port)
{
	sockaddr_in		address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	address.sin_port = htons(port);
	if (::connect(_sock, (sockaddr*)&address, sizeof(address)) == -1)
		return perror(E_SOCK_CONNECT), false;
	return true;
}

bool Socket::send(const std::string& data)
{
	if (::send(_sock, data.c_str(), data.size(), 0) == -1)
		return perror(E_SOCK_SEND), false;
	return true;
}

bool Socket::receive(std::string& data)
{
	char	buffer[BUFFERSIZE];
	int		readlen;

	readlen = ::recv(_sock, buffer, BUFFERSIZE, 0);
	if (readlen == -1)
		return perror(E_SOCK_RECEIVE), false;
	data = std::string(buffer, readlen);
	return true;
}

void Socket::close()
{
	if (_sock != -1)
		::close(_sock);
	_sock = -1;
}
