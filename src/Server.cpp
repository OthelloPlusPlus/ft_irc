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
// int	stoi(const char *)
#include <sys/socket.h>
//	int		socket(int domain, int type, int protocol);
//	ssize_t	recv(int socket, void *buffer, size_t length, int flags);
//	int		bind(int socket, const struct sockaddr *address, socklen_t address_len);
//	int		listen(int socket, int backlog);
#include <fcntl.h>
//	int	fcntl(int fildes, int cmd, ...);
#include <unistd.h>
//	int	close(int fildes);
#include <ifaddrs.h>
// int	getifaddrs(ifaddrs **)
// void	freeifaddrs(ifaddrs *)
#include <arpa/inet.h>
// char	*inet_ntoa(in_addr)

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

// Server::Server(void)
// {
// 	std::cout	<< C_DGREEN	<< "Default constructor "
// 				<< C_GREEN	<< "Server"
// 				<< C_DGREEN	<< " called."
// 				<< C_RESET	<< std::endl;
// }
Server::Server(int argc, char **argv)
{
	if (argc < 3)
		throw (std::range_error("Not enough aruments passed."));
	if (argc > 3)
		throw (std::range_error("Too many arguments passed."));
	this->port = std::stoi(argv[1]);
	this->password = argv[2];
	this->ip = this->getHostIp();
	this->bootUpServer();

	
	// std::cout	<< this->validatePassword("password ")	<< std::endl;
	// std::cout	<< "IP is:\t"	<< this->ip	<< std::endl;
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
	close(this->pollInfo.fd);
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

void	Server::bootUpServer(void)
{
	std::cout	<< C_BOLD	<< "Setting up server:\n"	<< C_RESET;
	std::cout	<< "Connecting socket..."	<< std::endl;	
	this->pollInfo.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->pollInfo.fd < 0)
		throw (std::runtime_error("socket(): "));
	if (fcntl(this->pollInfo.fd, F_SETFL, fcntl(this->pollInfo.fd, F_GETFL, 0) | O_NONBLOCK) == -1)
	{
		close(this->pollInfo.fd);
		throw (std::runtime_error("fcntl(): "));
	}
	this->socketAddress.sin_family = AF_INET;
	this->socketAddress.sin_port = htons(this->port);
	this->socketAddress.sin_addr.s_addr = inet_addr(this->ip.c_str());
	if (bind(this->pollInfo.fd, (struct sockaddr *)&this->socketAddress, sizeof(this->socketAddress)))
	{
		close(this->pollInfo.fd);
		throw (std::runtime_error("bind(): "));
	}
	if (listen(this->pollInfo.fd, SOMAXCONN))
	{
		close(this->pollInfo.fd);
		throw (std::runtime_error("listen(): "));
	}
	this->pollInfo.events = POLLIN;
}

std::string	Server::getHostIp(void) const
{
	std::string		ip;
	struct ifaddrs	*ifap0, *ifap;

	if (getifaddrs(&ifap0))
		throw(std::runtime_error("Couldn't get address."));
	for (ifap = ifap0; ifap != nullptr; ifap = ifap->ifa_next)
		if (ifap->ifa_addr && ifap->ifa_addr->sa_family == AF_INET)
			ip = inet_ntoa(((struct sockaddr_in *)ifap->ifa_addr)->sin_addr);
	freeifaddrs(ifap);
	return (ip);
}

bool	Server::validatePassword(const std::string password) const
{
	return (this->password == password);
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
