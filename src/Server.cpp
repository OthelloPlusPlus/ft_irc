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
	std::cout	<< std::left
				<< C_HEADER	<< std::setw(76)	<< "Setting up server "
				<< C_RESET	<< "\n";

	this->pollInfo.fd = -1;
	if (argc < 3)
		throw (std::range_error("Not enough aruments passed."));
	if (argc > 3)
		throw (std::range_error("Too many arguments passed."));
	this->port = std::stoi(argv[1]);
	this->password = argv[2];
	this->ip = this->getHostIp();
	this->bootUpServer();
	
	std::cout	<< "\n"
				<< C_HEADER	<< std::setw(76)	<< "Server setup complete"	<< C_RESET	<< "\n"
				<< std::setw(16)	<< " - IP address: "	<< this->ip	<< "\n"
				<< std::setw(16)	<< " - Port: "	<< this->port	<< "\n"
				<< " - Ready to receive incoming users!"
				<< std::right	<< std::endl;
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
	if (this->pollInfo.fd != -1)
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


/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */

Server	&Server::operator=(const Server &src)
{
	if (this == &src)
		return (*this);
	this->pollInfo = src.pollInfo;
	this->socketAddress = src.socketAddress;
	this->clients = src.clients;
	this->port = src.port;
	this->password = src.password;
	this->ip = src.ip;
	return (*this);
}
