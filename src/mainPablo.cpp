Skip to content
Search or jump to…
Pull requests
Issues
Codespaces
Marketplace
Explore
 
@mulain 
pandaero
/
42_Core_Webserv
Public
Fork your own copy of pandaero/42_Core_Webserv
Code
Issues
Pull requests
Actions
Projects
Wiki
Security
Insights
42_Core_Webserv/src/main.cpp
@pandaero
pandaero achieved a working server, need to work on responses and sending data…
…
Latest commit 34d84f1 2 days ago
 History
 1 contributor
71 lines (55 sloc)  1.58 KB

#include "../include/webserv.hpp"
#include "../include/Socket.hpp"

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/resource.h>

int	g_maxPollFds = 0;

void	handleHttpRequest(int sockfd)
{
	char				buffer[1024];
	struct sockaddr_in	client_address;
	socklen_t			client_address_length = sizeof(client_address);
	int					bytesRcvd = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_length);
	
	if (bytesRcvd == -1)
		std::cerr << "Error: did not receive data from socket." << std::endl;

	std::cout << "Received: " << buffer;

	std::string	received(buffer);
	
	std::string	response = "Received: \"" + received.substr(0, received.size() - 2) + "\" successfully.\n";
	send(sockfd, response.c_str(), response.size(), 0);
}

int	main(int argc, char **argv)
{

	if (argc > 2)
	{
		std::cerr << "Error: too many arguments." << std::endl;
		return (EXIT_FAILURE);
	}

	struct rlimit rlim;
	if (getrlimit(RLIMIT_NOFILE, &rlim) == -1) {
		std::cerr << "Error: could not get maximum number of file descriptors." << std::endl;
		return 1;
	}
	g_maxPollFds = rlim.rlim_cur;

	if (argc == 2) // Parse config file
		(void) argv;
	else // Use default config
		(void) argv;

	Socket	http;
	http.assignAddress("127.0.0.1");
	http.bind(9445);
	http.listen();

	Socket	whatevs;
	whatevs.assignAddress("127.0.0.1");
	whatevs.bind(9334);
	whatevs.listen();

	Socket	whatevselse;
	whatevselse.assignAddress("127.0.0.1");
	whatevselse.bind(9556);
	whatevselse.listen();

	while (true)
		Socket::poll();

	return (EXIT_SUCCESS);
}
Footer
© 2023 GitHub, Inc.
Footer navigation
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
42_Core_Webserv/main.cpp at listening-socket-creation · pandaero/42_Core_Webserv