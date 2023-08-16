/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/09 20:45:12 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "colors.hpp"

#include <iostream>
// std::
#include <unistd.h>
//	int	close(int fildes);

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

Client::Client(void)
{
	std::cout	<< C_DGREEN	<< "Default constructor "
				<< C_GREEN	<< "Client"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

Client::Client(const Client &src)
{
	*this = src;
	std::cout	<< C_DGREEN	<< "Copy constructor "
				<< C_GREEN	<< "Client"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

Client::~Client(void)
{
	std::cout	<< C_DRED	<< "Deconstructor "
				<< C_RED	<< "Client"
				<< C_DRED	<< " called"
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

void	Client::initialize(int serverFD)
{
	this->socketAddressLen = sizeof(this->socketAddress);
	this->pollInfo.fd = accept(serverFD, (struct sockaddr *)&this->socketAddress, &this->socketAddressLen);
	if (this->pollInfo.fd < 0)
		throw (std::runtime_error("accept(): "));
	this->pollInfo.events = POLLIN;
	
	// this->username = "Othello";
	this->sendMsg(":Othello!~Othello JOIN #WelcomeChannel\r\n");
	this->sendMsg(":Bot!communicate@localhost PRIVMSG #WelcomeChannel :Welcome to our ft_irc!\r\n");
}

void    Client::sendMsg(std::string msg)
{
    // std::cout    << "About to send:\t"   << msg  << std::flush;
    std::cout   << "send [" << send(this->pollInfo.fd, msg.c_str(), msg.length(), 0)
                << "]\t"    << msg      << std::endl;
}

void	Client::getMsg(void)
{
	if (poll(&this->pollInfo, 1, 0) < 0)
	{
		std::cerr	<< "Error poll(): "	<< strerror(errno)	<< std::endl;
		return ;
	}
	if (this->pollInfo.revents & POLLIN)
	{
		char	buffer[4096];
		ssize_t	recvLen;

		bzero(buffer, sizeof(buffer));
		recvLen = recv(this->pollInfo.fd, buffer, sizeof(buffer) - 1, 0);
		if (recvLen < 0)
			std::cerr	<< "Error recv(): "	<< strerror(errno)	<< std::endl;
		if (recvLen == 0)
		{
			close(this->pollInfo.fd);
			this->pollInfo.fd = 0;
			std::cout	<< "Client disconnected from server."	<< std::endl;
		}
		else
		{
			std::cout	<< buffer	<< std::endl;
			// this->sendMsg(":Bot!communicate@localhost NOTICE Othello Message received\r\n");
			this->sendMsg(":Bot!communicate@localhost NOTICE Othello :Message received\r\n");
		}
	}
}

bool	Client::stillActive(void) const
{
	return (this->pollInfo.fd > 0);
}

// void	Client::printInfo(void) const
// {
// 	std::cout	<< "socketAddress.sin_addr.s_addr\t"	<< this->socketAddress.sin_addr.s_addr	<< "\n"
// 				<< "PollInfo.fd\t"	<< this->pollInfo.fd	<< "\n"
// 				<< std::flush;
// }

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */

Client	&Client::operator=(const Client &src)
{
	if (this == &src)
		return (*this);
	this->socketAddress = src.socketAddress;
	this->socketAddressLen = src.socketAddressLen;
	this->pollInfo = src.pollInfo;
	return (*this);
}
