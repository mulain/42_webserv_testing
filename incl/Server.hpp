#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <vector>
#include <arpa/inet.h>

#include <Utils.hpp>

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
	- should be part of the constructor. prolly take argv1 as arg 
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
		// Utils
		

		
		in_addr_t		_ipAddress;
		uint16_t		_port;

		//prolly shite:
		HTTPrequest		_request;

};

#endif
