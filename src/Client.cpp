/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:51:05 by pandalaf          #+#    #+#             */
/*   Updated: 2023/03/31 22:04:04 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

#include <unistd.h>

Client::Client()
{

}

Client::Client(const Client & other):
	_clientSocketfd(other._clientSocketfd),
	_clientSockLen(other._clientSockLen),
	_clientAddress(other._clientAddress)
{

}

Client::~Client()
{

}

Client &	Client::operator=(const Client & other)
{
	if (this != &other)
	{
		_clientSocketfd = other._clientSocketfd;
		_clientSockLen = other._clientSockLen;
		_clientAddress = other._clientAddress;
	}
	return (*this);
}

int	Client::getSocketfd()
{
	return (_clientSocketfd);
}

void	Client::accept(int serverSocket)
{
	_clientSocketfd = ::accept(serverSocket, (struct sockaddr *) &_clientAddress, &_clientSockLen);
	if (_clientSocketfd == -1)
	{
		close(_clientSocketfd);
		throw connectionDeniedException();
	}
}

const char *	Client::connectionDeniedException::what() const throw()
{
	return ("Error: connection with the client was denied.");
}