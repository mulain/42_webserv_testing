#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <cstring>

#include "Utils.hpp"
#include "Socket.hpp"

#define MAXCONNECTS		32
#define BUFFERSIZE		1024
#define E_NUM_CONNECTS	"Maximum number of connections reached."

typedef std::vector<Socket>::const_iterator	SocketVec_it;


typedef struct HTTPrequest
{
	std::string		Method;
	std::string		URI;
	std::string		HTTPversion;
	StringMap		Headers;
	std::string		Body;
}	HTTPrequest;


/*
needed functions:
- build server function that parses the config file 
	- should be part of the constructor. prolly take argv1 as arg and then parse and build
		from there
Server hard coded for now to 127.0.0.1:3000 with backlog of 10.


thoughts
- should the server immediately start listening upon creation, i.e. should the constrcutor
	already do the listengin shit? i think yes atm, cause why would we not want the server to listen
	in the context of this project?
*/
class Server
{
	public:
		Server(void);
		~Server(void);
		Server(const Server&);

		Server& operator=(const Server&);

		// Actions
		bool	listen();
		Socket&	newSocket();
		int		acceptConnection(); //adds a new socket and accepts connection on dat mofo
		void	poll();
		void	check_listeningSocket();
		void	check_clientSockets();
		void	destroySocket(size_t);

		//Crap, move this elsewhere / integrate in ackchual function
		void parseRequest(std::string);
		void printRequest() const;
		HTTPrequest getRequest() const;
	
	private:		
		in_addr_t			_ipAddress;
		uint16_t			_port;
		size_t				_backlog;
		Socket				_listeningSocket; //only ever have the one
		std::vector<Socket>	_connections; //have as many as needed / up to a max maybe
		pollfd				_pollStructs[MAXCONNECTS];
		
		//prolly shite:
		HTTPrequest		_request;

};

#endif
