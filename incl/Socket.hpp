#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#define E_SOCK_CREATE	"Could not create socket"
#define E_SOCK_BIND		"Could not bind address to socket"
#define E_SOCK_LISTEN	"Could not listen for connection on socket"
#define E_SOCK_ACCEPT	"Could not accept connection on socket"
#define E_SOCK_CONNECT	"Could not connect to remote socket"
#define E_SOCK_SEND		"Could not send over socket"
#define E_SOCK_RECEIVE	"Could not receive from socket"

class Socket
{
	public:
		Socket(void); //Default constructor will create TCP socket (SOCK_STREAM)
		Socket(int);
		~Socket(void);
		Socket(const Socket&);

		Socket& operator=(const Socket&);

		bool	create(int);
		bool	bind(const std::string&, int); //takes unprocessed input
		bool	bind(in_addr_t, uint16_t); //overload for already processed input
		bool	listen(int);
		bool	accept(int);
		bool	connect(const std::string&, int);
		bool	send(const std::string&);
		bool	receive(std::string&);
		void	close();
		int		getSocketFd() const;
	
	private:
		int				_socketfd;
		sockaddr_in		_address;
};

#endif
