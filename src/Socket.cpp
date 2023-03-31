

#include "../incl/Socket.hpp"

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>


std::map<int, std::vector<Client> >	Socket::_connectionMap;
std::vector<int>					Socket::_serverSocketfds;
std::vector<int>					Socket::_clientSocketfds;
struct pollfd * 					Socket::_pollStruct = new struct pollfd[MAX_FD_COUNT];
int									Socket::_numPolledfds = 0;

void	Socket::poll()
{
	int polling = ::poll(_pollStruct, _numPolledfds, -1);
	if (polling == -1)
	{
		for (size_t i = 0; i < _serverSocketfds.size(); ++i)
			close(_serverSocketfds[i]);
		std::cerr << "Error: could not start polling on the sockets." << std::endl;
	}

	for (int i = 0; i < _numPolledfds; ++i)
	{
		if (_pollStruct[i].revents & POLLIN)
		{
			std::cout << "Ready to receive." << std::endl;
			if (std::find(_serverSocketfds.begin(), _serverSocketfds.end(), _pollStruct[i].fd) != _serverSocketfds.end())
			{
				if (_connectionMap.find(_pollStruct[i].fd) != _connectionMap.end())
				{
					Client	newClient;
					newClient.accept(_pollStruct[i].fd);
					std::cout << "New client connected." << std::endl;
					_connectionMap.find(_pollStruct[i].fd)->second.push_back(newClient);
					_clientSocketfds.push_back(newClient.getSocketfd());
					_pollStruct[_numPolledfds].fd = newClient.getSocketfd();
					_pollStruct[_numPolledfds++].events = POLLIN;
				}
			}
			else
			{
				std::cout << "Receiving..." << std::endl;
				char	buffer[1024];
				size_t	bytesReceived = recv(_pollStruct[i].fd, buffer, 1024, 0);
				if (bytesReceived <= 0)
				{
					close(_pollStruct[i].fd);
					_clientSocketfds.erase(find(_clientSocketfds.begin(), _clientSocketfds.end(), _pollStruct[i].fd));
					--_numPolledfds;
				}
				else
				{
					std::cout << "Received " << bytesReceived << " bytes from client. Message: " << buffer << "." << std::endl;
					int	bytesSent = send(_pollStruct[i].fd, buffer, bytesReceived, 0);
					if (bytesSent == -1)
						std::cerr << "Error: could not send data to client" << std::endl;
				}
			}
		}
	}
}

Socket::Socket():
	_socketfd(-1)
{
	int	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1)
		throw socketCreationFailureException();
	_pollStruct[_numPolledfds].revents = 0;
	_pollStruct[_numPolledfds].events = POLLIN;
	_pollStruct[_numPolledfds++].fd = socketfd;
	new (this) Socket(socketfd);
}

Socket::Socket(const Socket & other):
	_socketfd(other._socketfd),
	_port(other._port)
{

}

Socket::~Socket()
{
	close(_socketfd);
}

Socket &	Socket::operator=(const Socket & other)
{
	if (this != &other)
		new (this) Socket(other);
	return (*this);
}

int	Socket::getPort() const
{
	return (_port);
}

int	Socket::getSocketfd() const
{
	return (_socketfd);
}

void	Socket::assignAddress(std::string address)
{
	if (address == "any")
		_serverAddress.sin_addr.s_addr = INADDR_ANY;
	else
	{
		int numAddress = inet_addr(address.c_str());
		if (numAddress == -1)
			throw invalidAddressException();	
		_serverAddress.sin_addr.s_addr = inet_addr(address.c_str());
	}
}

void	Socket::bind(int port)
{
	_serverAddress.sin_port = htons(port);
	if (fcntl(_socketfd, F_SETFL, O_NONBLOCK) < 0)
		perror("fcntl");
	if (::bind(_socketfd, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress)) == -1)
	{
		close(_socketfd);
		throw bindingFailureException();
	}
}

void	Socket::listen()
{
	if (::listen(_socketfd, SOMAXCONN) == -1)
	{
		close(_socketfd);
		throw listenFailureException();
	}
}

Socket::Socket(int socketfd):
	_socketfd(socketfd)
{
	_serverAddress.sin_family = AF_INET;
	_serverSocketfds.push_back(socketfd);
	std::vector<Client>	emptyVector;
	_connectionMap.insert(make_pair(socketfd, emptyVector));
}

const char *	Socket::socketCreationFailureException::what() const throw()
{
	return("Error: socket: failed to create socket.");
}

const char *	Socket::bindingFailureException::what() const throw()
{
	return("Error: socket: failed to bind socket to port.");
}

const char *	Socket::listenFailureException::what() const throw()
{
	return("Error: socket: failed to listen on port.");
}

const char *	Socket::invalidAddressException::what() const throw()
{
	return("Error: socket: invalid address.");
}

/*
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
	
	return *this;
}


#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
	long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char *hello = "Hello from server";
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
	*/