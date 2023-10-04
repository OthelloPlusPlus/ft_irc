/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   BotTicTacToe.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/09 20:45:12 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "BotTicTacToe.hpp"
#include "Command.hpp"
#include "Parse.hpp"
#include "verboseCheck.hpp"
#include "colors.hpp"

#include <iostream>
// std::
#include <unistd.h>
// int pipe(int pipefd[2]);
// int close(int fd);
#include <strings.h>
// void bzero(void *s, size_t n);
#include <string.h>
// char *strerror(int errnum);
#include <tuple>
// std::tuple

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

BotTicTacToe::BotTicTacToe(Server &server): AClient(server)
{
	this->_nickName = "TicTacBot";
	this->_userName = "TicTacToeBot";
	this->_realName = "Tic-Tac-Toe playing Bot";
	this->_isRegistered = true;
	this->_isOperator = false;
	// if (pipe(this->pipeFD) == -1)
	// 	throw(std::runtime_error("pipe(): "));
	std::cout	<< C_DGREEN	<< "Default constructor "
				<< C_GREEN	<< "BotTicTacToe"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

// BotTicTacToe::BotTicTacToe(const BotTicTacToe &src)
// {
// 	*this = src;
// 	std::cout	<< C_DGREEN	<< "Copy constructor "
// 				<< C_GREEN	<< "BotTicTacToe"
// 				<< C_DGREEN	<< " called."
// 				<< C_RESET	<< std::endl;
// }

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

BotTicTacToe::~BotTicTacToe(void)
{
	// if (this->pipeFD[0] > 2)
	// 	close(this->pipeFD[0]);
	// if (this->pipeFD[1] > 2)
	// 	close(this->pipeFD[1]);
	this->closeFD();
	std::cout	<< C_DRED	<< "Deconstructor "
				<< C_RED	<< "BotTicTacToe"
				<< C_DRED	<< " called"
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

bool	BotTicTacToe::stillActive(void) const
{
	return (true);
}

std::string	BotTicTacToe::getMsg(void)
{
	std::string	reply;
	std::size_t	pos;

	this->readReceive();
	pos = this->_buffer.find('\n');
	if (pos != std::string::npos)
	{
		reply = this->botRespond(this->_buffer.substr(0, pos + 1));
		this->_buffer.erase(0, pos + 1);
	}
	return (reply);
}

bool	BotTicTacToe::readReceive(void)
{
	while (!this->msgs.empty())
	{
		size_t spacePos = this->msgs.front().find(' ');
		std::tuple<AClient& , std::string, std::vector<std::string>> fwd = \
		Parse::parseMsg(*(dynamic_cast<AClient *>(this)), this->msgs.front().substr(spacePos + 1));
		std::cout	<< "bot recv\t"	<< this->msgs.front() << std::endl;
		std::cout	<< "Nick\t" << std::get<0>(fwd).getNickName() << std::endl;
		std::cout	<< "cmd\t" << std::get<1>(fwd) << std::endl;
		std::cout	<< "args\t" << std::get<2>(fwd)[0] << std::endl;
		std::cout	<< "args\t" << std::get<2>(fwd)[1] << std::endl;
		std::cout	<< '\t' << std::endl;
		this->msgs.pop();
	}
	// char	buffer[4096];
	// ssize_t	recvLen;
	
	// bzero(buffer, sizeof(buffer));
	// std::cout	<< __func__	<< __LINE__	
	// 			<< '\t'	<< this->pipeFD[0]	<< '\t'	<< this->pipeFD[1]
	// 			<< std::endl;

	// recvLen = recv(this->pipeFD[0], buffer, sizeof(buffer) - 1, 0);
	// std::cout	<< __func__	<< __LINE__	
	// 			<< '\t'	<< recvLen
	// 			<< std::endl;
	// if (recvLen < 0)
	// {
	// 	std::cerr	<< "Error read(): "	<< strerror(errno)	<< std::endl;
	// 	return (false);
	// }
	// std::cout	<< __func__	<< __LINE__	<< std::endl;
	// this->_buffer.append(buffer);
	return (true);
}

std::string	BotTicTacToe::botRespond(std::string msg)
{
	std::cout	<< "[Think]\t"	<< msg	<< std::endl;
	return ("");
}

void	BotTicTacToe::sendMsg(std::string msg)
{
	this->msgs.push(msg);
	// msg = ':' + this->_server.getName() + ' ' + msg + "\r\n";

	// ssize_t	size = send(this->pipeFD[0], msg.c_str(), msg.length(), 0);
	// if (verboseCheck() >= V_MSG)
	// 	std::cout	<< C_RESET	<< "Send ["	<< size	<< "]\t"
	// 				<< C_LORANGE	<< msg
	// 				<< C_RESET	<< std::flush;
}

void	BotTicTacToe::closeFD(void)
{
	while (!this->msgs.empty())
		this->msgs.pop();
	// if (this->pipeFD[0] > 2)
	// {
	// 	close(this->pipeFD[0]);
	// 	this->pipeFD[0] = -1;
	// }
	// if (this->pipeFD[1] > 2)
	// {
	// 	close(this->pipeFD[1]);
	// 	this->pipeFD[1] = -1;
	// }
}

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */

// BotTicTacToe	&BotTicTacToe::operator=(const BotTicTacToe &src)
// {
// 	if (this == &src)
// 		return (*this);

// 	return (*this);
// }
