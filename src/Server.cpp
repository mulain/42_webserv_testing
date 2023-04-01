
#include "../incl/Server.hpp"

// Constructors and destructors
Server::Server(void)
{
	_ipAddress = inet_addr("10.12.11.50");
	_port = htons(3000);
	_backlog = 10;
	for (int i = 1; i < MAXCONNECTS + 1; i++)
		_pollStructs[i].fd = -1;
	listen();
}

Server::~Server(void)
{}

Server::Server(const Server& src)
{
	*this = src;
}

// Operator overloads
Server& Server::operator=(const Server& src)
{
	//incomplete af
	_request = src._request;
	return *this;
}

// Public member functions
bool Server::listen()
{
	_listeningSocket.bind(_ipAddress, _port);
	_pollStructs[0].fd = _listeningSocket.getSocketFd();
	_pollStructs[0].events = POLLIN;
	_listeningSocket.listen(_backlog);
	return true; //just for now. prolly change all bools to throws ?
}

Socket& Server::newSocket()
{
	Socket	new_socket;

	if (_connections.size() >= MAXCONNECTS)
		std::cerr << E_NUM_CONNECTS << std::endl;
	_connections.push_back(new_socket);
	std::cout << "Number of sockets: " << _connections.size() << std::endl;
	return _connections.back();
}

int Server::acceptConnection()
{
	Socket& newSock = newSocket();
	newSock.accept(_listeningSocket.getSocketFd());
	return newSock.getSocketFd();
}

#include <cstring>
bool Server::poll()
{
	::poll(_pollStructs, MAXCONNECTS + 1, -1); //Deal with timeout (3rd arg)
	
	// [0] is the listening socket's pollStruct. Handles new connections
	if (_pollStructs[0].revents & POLLIN)
	{
		int newConnFd = acceptConnection();
		const char *msg = "HTTP/1.1 200 OK\r\nContent-Type: html\r\nContent-Length: 104\r\n\r\n<!DOCTYPE html><html><head><title>Hello, world!</title></head><body><h1>Hello, world!</h1></body></html>";
		write(newConnFd, msg, strlen(msg));
		for (int i = 1; i < MAXCONNECTS + 1; i++)
		{
			if (_pollStructs[i].fd == -1)
			{
				_pollStructs[i].fd = newConnFd;
				_pollStructs[i].events = POLLIN;
				break;
			}
			if (i == MAXCONNECTS + 1)
				std::cerr << E_NUM_CONNECTS;
		}
	}
	// cycles thru all other pollstructs and evals those that have valid fds
	for (int i =1; i < MAXCONNECTS + 1; i++)
	{
		if (_pollStructs[i].fd == -1)
			continue;
		if (_pollStructs[i].revents & POLLIN)
		{
			char buffer[BUFFERSIZE] = {0};
			int num_bytes = recv(_pollStructs[i].fd, buffer, sizeof(buffer), 0);
			if (!num_bytes)
			{
				close(_pollStructs[i].fd);
				_pollStructs[i].fd = -1;
			}
			else
				write(1, buffer, num_bytes);
		}
	}
	return true;
}

void Server::printRequest() const
{
	std::cout << "Method: " << _request.Method << "\n";
	std::cout << "URI: " << _request.URI << "\n";
	std::cout << "HTTP: " << _request.HTTPversion << std::endl;
	std::cout << "Headers:" << std::endl;
	for (StringMap_it it = _request.Headers.begin(); it != _request.Headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << "Body: " << _request.Body << std::endl;
}

void Server::parseRequest(std::string requestString)
{
	// remove possible leading whitespace that is not syntax relevant (\r\n are relevant!)
	// maybe remove this because this shouldnt actually happen
	requestString.erase(0, requestString.find_first_not_of(" \t\v"));

	// parse request line
	_request.Method = Utils::splitEraseStr(requestString, " ");	
	_request.URI = Utils::splitEraseStr(requestString, " ");
	_request.HTTPversion = Utils::splitEraseStr(requestString, "\r\n");

	// parse headers
	_request.Headers = Utils::splitEraseStrMap(requestString, ": ", "\n", '\n');

	// parse body (which is just what remains of the input minus the preceding newline)
	if (requestString[0] == '\n')
		requestString.erase(0, 1);
	_request.Body = requestString;
}
// GETTERS

HTTPrequest Server::getRequest() const
{
	return _request;
}
