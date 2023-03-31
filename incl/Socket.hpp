#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define E_SOCK_CREATE	"Could not create socket."
#define E_SOCK_BIND		"Could not bind address to socket."
#define E_SOCK_LISTEN	"Could not listen for connection on socket."
#define E_SOCK_ACCEPT	"Could not accept connection on socket."

class Socket
{
	public:
		Socket(void);
		~Socket(void);
		Socket(const Socket&);

		Socket& operator=(const Socket&);

		bool create(int);
		bool bind(const std::string&, int);
		bool listen(int);
		bool accept(Socket&);
		bool connect(const std::string&, int);
		bool send(const std::string&);
		bool receive(std::string&);
		void close();
	
	private:
		int	_sock
};

#endif
