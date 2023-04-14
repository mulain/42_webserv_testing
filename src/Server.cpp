
#include "../incl/Server.hpp"

// Constructors and destructors
Server::Server(void)
{
	_host = inet_addr("10.12.11.50");
	_port = htons(3000);
	_backlog = 100;
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
	for (size_t i = 0; i < MAXCONNECTS + 1; i++)
	{
		_pollStructs[i].fd = src._pollStructs[i].fd;
		_pollStructs[i].events = src._pollStructs[i].events;
		_pollStructs[i].revents = src._pollStructs[i].revents;
	}
	return *this;
}

// Public member functions

// Setters
void Server::setName(std::string input)
{
	_name = input;
}

void Server::setHost(std::string input)
{
	_host = inet_addr(input.c_str());
}

void Server::setPort(std::string input)
{
	//use strtol and throw exception
	_port = htons(atoi(input.c_str()));
}

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
		std::cerr << E_NUM_CONNECTS << std::endl; //throw 
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
void Server::poll()
{
	::poll(_pollStructs, MAXCONNECTS + 1, -1); //Deal with timeout (3rd arg)
	check_listeningSocket();
	check_clientSockets();
}

/*
_pollStructs[0] is the listening socket's poll struct
*/
void Server::check_listeningSocket()
{
	if (!(_pollStructs[0].revents & POLLIN)) //what other events are important for listener? Also have to configure the setup if change nevessary
		return;
	int newConnFd = acceptConnection();
	
	//send hello msg / acknowledge connection
	const char *msg = "HTTP/1.1 200 OK\r\nContent-Type: html\r\nContent-Length: 104\r\n\r\n<!DOCTYPE html><html><head><title>Hello, world!</title></head><body><h1>Hello, world!</h1></body></html>";
	write(newConnFd, msg, strlen(msg));
	
	//assign an available pollstruct to the new connection
	for (size_t i = 1; i < MAXCONNECTS + 1; i++)
	{
		if (_pollStructs[i].fd == -1)
		{
			_pollStructs[i].fd = newConnFd;
			_pollStructs[i].events = POLLIN; //add other event types
			break;
		}
		if (i == MAXCONNECTS + 1)
		{
			std::cerr << E_NUM_CONNECTS << std::endl;
			//send error to client
			destroySocket(newConnFd);
		}
	}
}

/*
POLLIN - socket has data to be read
POLLOUT - socket is ready to write stuff to client
POLLPRI - urgent data to be read (so similar to POLLIN)
POLLNVAL - socket fd is not open or some shit
POLLERR - socketerr (transmission error or protocol violation)
POLLHUP - other end (client) hung up on our ass
*/
void Server::check_clientSockets()
{
	// cycles thru pollstructs omitting socket 0 (listening socket)
	// and evals those that have valid fds
	for (size_t i = 1; i < MAXCONNECTS + 1; i++)
	{	
		// poll struct is not in use
		if (_pollStructs[i].fd == -1)
			continue;
		
		// poll struct got canceled / encountered an error
		if (_pollStructs[i].revents & (POLLHUP | POLLERR | POLLNVAL))
			destroySocket(i);

		// poll struct detected incoming data
		if (_pollStructs[i].revents & POLLIN) // other events!
		{
			char buffer[BUFFERSIZE] = {0};
			int num_bytes = recv(_pollStructs[i].fd, buffer, sizeof(buffer), 0);
			if (!num_bytes)
				destroySocket(_pollStructs[i].fd);
			else
				std::cout << "checkClients POLLIN event." << std::endl;
				//write(1, buffer, num_bytes);
				//handle request
		}
	}
}

void Server::destroySocket(size_t index)
{
	for (SocketVec_it it = _connections.begin(); it != _connections.end(); it++)
	{
		if (it->getSocketFd() == _pollStructs[index].fd)
		{
			std::cout << "destroySocket: Address of socket in vector: " << &it << std::endl;
			std::cout << "getSocketFd: " << it->getSocketFd() << std::endl;
			_connections.erase(it); //vector calls the socket destructor which closes the fd
			_pollStructs[index].fd = -1;
			break;
		}
	}
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
