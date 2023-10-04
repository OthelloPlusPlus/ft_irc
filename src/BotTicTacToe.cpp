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

	if (!this->msgs.empty())
	{
		size_t	pos = this->msgs.front().find(' ') + 1;
		std::tuple<AClient &, std::string, std::vector<std::string>> prsd = \
			Parse::parseMsg(*this, this->msgs.front().substr(pos));
		reply = this->botRespond(std::get<0>(prsd), std::get<1>(prsd), std::get<2>(prsd));
		this->msgs.pop();
	}
	if (!reply.empty())
	std::cout	<< "bot reply\t["	<< reply	<< ']'	<< std::endl;
	return (reply);
}

std::string	BotTicTacToe::botRespond(AClient &src, const std::string cmd, const std::vector<std::string> &args)
{
	if (cmd == "INVITE")
		return (this->botRespondInvite(args));
	if (cmd == "PART")
		return (this->botRespondPart(args));
		// return ("JOIN " + args[0]);
		// std::cout << "I was invited to the party!"	<< std::endl;
	return ("");
}

std::string	BotTicTacToe::botRespondInvite(const std::vector<std::string> &args)
{
	Channel *channel = this->_server.getChannel(args[1]);

	if (channel == nullptr)
		return ("");
	return ("JOIN " + channel->getName() + "\r\n");
}

std::string	BotTicTacToe::botRespondPart(const std::vector<std::string> &args)
{
	Channel *channel = this->_server.getChannel(args[0]);

	if (channel != nullptr && channel->getSize() <= 1)
		return ("PART " + channel->getName() + "\r\n");
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
