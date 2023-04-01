#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <vector>
#include <arpa/inet.h>

#include "Utils.hpp"
#include "Socket.hpp"

typedef std::map<std::string, std::string>::const_iterator StringMap_it;
typedef std::map<std::string, std::string> StringMap;

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
Server hard coded for now to 127.0.0.1:3000


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
		bool listen();
		
		void parseRequest(std::string);
		void printRequest() const;

		// Getters
		HTTPrequest getRequest() const;
	
	private:		
		in_addr_t		_ipAddress;
		uint16_t		_port;
		Socket			_listeningSocket;

		//prolly shite:
		HTTPrequest		_request;

};

#endif
