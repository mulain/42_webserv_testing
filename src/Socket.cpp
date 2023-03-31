
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

// public member functions

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

bool accept(Socket& newSocket)
{
	sockaddr_in		address;
	socklen_t		len = sizeof(address);

	newSocket._sock = ::accept(_sock, &address, &len);
	if (newSocket._sock == -1)
		return perror(E_SOCK_ACCEPT), false;
	return true;
}

bool connect(const std::string& ipAddress, int port)
{
	
}


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
	