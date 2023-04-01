
#include "../incl/Server.hpp"

// Constructors and destructors
Server::Server(void)
{
	_ipAddress = inet_addr("127.0.0.1");
	_port = htons(3000);
	_backlog = 10;
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
	_listeningSocket.listen(_backlog);
	return true; //just for now. prolly change all bools to throws ?
}

Socket& Server::newSocket()
{
	Socket	new_socket;

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
