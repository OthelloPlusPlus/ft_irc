/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/09 20:45:12 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "colors.hpp"

#include <iostream>
// std::
#include <fcntl.h>
//	int	fcntl(int fildes, int cmd, ...);
#include <arpa/inet.h>
//	in_addr_t	inet_addr(const char *cp);
//	uint16_t	htons(uint16_t hostshort);
#include <unistd.h>
//	int	close(int fildes);
/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

Server::Server(void)
{
	std::cout	<< C_DGREEN	<< "Default constructor "
				<< C_GREEN	<< "Server"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

Server::Server(const Server &src)
{
	*this = src;
	std::cout	<< C_DGREEN	<< "Copy constructor "
				<< C_GREEN	<< "Server"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

Server::~Server(void)
{
	std::cout	<< C_DRED	<< "Deconstructor "
				<< C_RED	<< "Server"
				<< C_DRED	<< " called"
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

bool	Server::initialize(void)
{
	try
	{
		this->socketAddress.sin_family = AF_INET;
		this->socketAddress.sin_port = htons(6667);
		this->socketAddress.sin_addr.s_addr = inet_addr("10.11.2.7"); //ipconfig getifaddr en0
		this->socketAddressLen = sizeof(this->socketAddress);
		this->setSocket();
		this->pollInfo.events = POLLIN;
	}
	catch(const std::exception &e)
	{
		close(this->pollInfo.fd);
		std::cerr << "Error "	<< e.what() << strerror(errno)	<< std::endl;
		return (false);
	}
	return (true);
}

void	Server::setSocket(void)
{
	std::cout	<< "Creating socket for incoming connections...\n";
	this->pollInfo.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->pollInfo.fd < 0)
		throw (std::runtime_error("socket(): "));
	std::cout	<< "Configuring socket for non-blocking mode...\n";
	if (fcntl(this->pollInfo.fd, F_SETFL, fcntl(this->pollInfo.fd, F_GETFL, 0) | O_NONBLOCK) == -1)
		throw (std::runtime_error("fcntl(): "));
	std::cout	<< "Binding socket to port "	<< ntohs(this->socketAddress.sin_port)	<< "...\n";
	if (bind(this->pollInfo.fd, (struct sockaddr *)&this->socketAddress, this->socketAddressLen) == -1)
		throw (std::runtime_error("bind(): "));
	std::cout	<< "Setting socket to listen for incoming connections...\n";
	if (listen(this->pollInfo.fd, SOMAXCONN) == -1)
		throw (std::runtime_error("listen(): "));
	std::cout	<< "Server setup complete. Ready to receive incoming users!"	<< std::endl;
}

bool	Server::checkNewClients(void)
{
	if (poll(&this->pollInfo, 1, 0) < 0)
	{
		std::cerr	<< "Error poll(): "	<< strerror(errno)	<< std::endl;
		return (false);
	}
	if (this->pollInfo.revents == 0)
		return (true);
	if (this->pollInfo.revents & POLLIN)
		this->acceptClient();
	return (true);
}

void	Server::acceptClient(void)
{

	try
	{
		std::cout	<< "Creating new client!"	<< std::endl;
		Client	*newClient = new Client(this->pollInfo.fd);
		// newClient->initialize(this->pollInfo.fd);
		// ipAddressFromSocketAddress(this->socketAddress)
		newClient->sendMsg(":localhost 375 Othello :- ft_irc Message of the Day - \r\n");
		newClient->sendMsg(":localhost 372 Othello :- We know what we're doing! We swear!\r\n");
		newClient->sendMsg(":localhost 376 Othello :End of /MOTD command.\r\n");
		this->clients.push_back(newClient);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

bool	Server::checkClients(void)
{
	size_t	i;

	i = this->clients.size();
	while (i > 0)
	{
		if (!this->clients[i - 1]->stillActive())
		{
			delete this->clients[i -1];
			this->clients.erase(this->clients.begin() + i - 1);
		}
		this->clients[i - 1]->getMsg();
		--i;
	}
	return (true);
}

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */

Server	&Server::operator=(const Server &src)
{
	if (this == &src)
		return (*this);

	return (*this);
}
