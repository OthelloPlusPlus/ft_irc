/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/08/17 20:57:15 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
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

Channel::Channel(std::string name): name(name), topic("")
{
	this->topic = "This topic is brought to you by HardCoded++";
	std::cout	<< C_DGREEN	<< "Default constructor "
				<< C_GREEN	<< "Channel"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

Channel::Channel(const Channel &src)
{
	*this = src;
	std::cout	<< C_DGREEN	<< "Copy constructor "
				<< C_GREEN	<< "Channel"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

Channel::~Channel(void)
{
	std::cout	<< C_DRED	<< "Deconstructor "
				<< C_RED	<< "Channel"
				<< C_DRED	<< " called"
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

std::string	Channel::getName(void) const
{
	return (this->name);
}

std::string	Channel::getTopic(void) const
{
	return (this->topic);
}

void	Channel::addClient(Client *newClient)
{
	this->clients.push_back(newClient);
	this->sendTopic(newClient);
	this->sendNames(newClient);
}

// void	Channel::sendMsgToChannel(Client *sender, std::string msg)
// {
// 	size_t	i;

// 	i = this->operators.size();
// 	while (i > 0)
// 	{
// 		if (this->operators[i - 1]->stillActive() && \
// 			this->operators[i - 1] != sender)
// 			this->operators[i - 1]->sendMsg(msg);
// 		--i;
// 	}
// 	i = this->clients.size();
// 	while (i > 0)
// 	{
// 		if (this->clients[i - 1]->stillActive() && \
// 			this->clients[i - 1] != sender)
// 			this->clients[i - 1]->sendMsg(msg);
// 		--i;
// 	}
// }


void	Channel::sendTopic(Client *client)
{
	std::string	msg;

	msg = ": 332 " + std::string("Othello") + " " + this->name + " :" + this->topic + "\r\n";
	client->sendMsg(msg);
}

void	Channel::sendNames(Client *client)
{
	std::string	msg;

	msg = ": 353 " + std::string("Othello") + " = " + this->name + " :";
	for (size_t i = this->operators.size(); i > 0; --i)
		msg += "@" + std::string("Othello") + " ";
	for (size_t i = this->clients.size(); i > 0; --i)
		msg += std::string("Othello") + " ";
	msg += "\r\n";
	client->sendMsg(msg);
	msg = ": 366 " + std::string("Othello") + " " + this->name + " :end of /Names list.\r\n";
	client->sendMsg(msg);
}

void	Channel::sendPrivMsg(Client *sender, std::string msg)
{
	msg = ":" + std::string("Othello") + " PRIVMSG " + this->name + " :" + msg + "\r\n";
	for (size_t i = this->operators.size(); i > 0; --i)
		if (this->operators[i - 1] != sender)
			this->operators[i - 1]->sendMsg(msg);
	for (size_t i = this->clients.size(); i > 0; --i)
		if (this->clients[i - 1] != sender)
			this->clients[i - 1]->sendMsg(msg);
// client->sendMsg(":Bot!communicate@localhost PRIVMSG #WelcomeChannel :Welcome to our ft_irc!\r\n");
}

void	Channel::sendWho(Client *client)
{
	std::string	msg;

	// msg = ": 352 " + std::string("Othello") + " " + this->name + " ";
	// for (size_t i = this->operators.size(); i > 0; --i)
	// {
	// 	std::string	msgWho;
	// 	msgWho = msg +	this->operators[i - 1].getIdentName() + " " + \
	// 					this->operators[i - 1].getHostName() + " " + \
	// 					this->operators[i - 1].getServer() + " " + \
	// 					this->operators[i - 1].getNick() + " H@:0 " + \
	// 					this->operators[i - 1].getRealName() + "\r\n";
	// 	client->sendMsg(msgWho);
	// }
	// for (size_t i = this->clients.size(); i > 0; --i)
	// {
	// 	std::string	msgWho;
	// 	msgWho = msg +	this->clients[i - 1].getIdentName() + " " + \
	// 					this->clients[i - 1].getHostName() + " " + \
	// 					this->clients[i - 1].getServer() + " " + \
	// 					this->clients[i - 1].getNick() + " H:0 " + \
	// 					this->clients[i - 1].getRealName() + "\r\n";
	// 	client->sendMsg(msgWho);
	// }
	msg = ": 315 " + std::string("Othello") + " " + this->name + " :End of /WHO list.\r\n";
	client->sendMsg(msg);
}

void	Channel::inviteClient(Client *client)
{
	std::string	msg;

	msg = std::string("Othello") + "1" + std::string("Othello") + "JOIN" + this->name + "\r\n";
	client->sendMsg(msg);
}

void	Channel::kickClient(Client *client)
{
	for (size_t i = this->clients.size(); i > 0; --i)
	{
		if (this->clients[i - 1] == client)
		{
			this->clients[i - 1]->sendMsg("PART " + this->name);
			this->clients.erase(this->clients.begin() + 1 - 1);
			return ;
		}
	}
}

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */

Channel	&Channel::operator=(const Channel &src)
{
	if (this == &src)
		return (*this);
	return (*this);
}
