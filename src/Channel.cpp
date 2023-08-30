/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/08/30 20:23:48 by ohengelm      ########   odam.nl         */
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

bool	Channel::userIsInChannel(const Client *client) const
{
	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
		if ((*i).client == client)
			return (true);
	return (false);
}

void	Channel::setTopic(ChannelUser user, const std::string newTopic)
{
	if (this->modeTopic == false || user.admin == false)
		return ;
	this->topic = newTopic;
}

void	Channel::addClient(Client *newClient)
{
	ChannelUser	newUser;

	if (this->userIsInChannel(newClient))
	{
		std::cout	<< C_LRED	<< "User "
					<< C_RESET	<< newClient->getNickName()
					<< C_LRED	" is already in channel "
					<< C_RESET	<< this->name	<< std::endl;
		return ;
	}
	newUser.client = newClient;
	newUser.admin = false;
	newUser.timestamp = 0;
	this->users.push_back(newUser);
	// std::cout	<< C_PURPLE	<< "JOIN message"	<< C_RESET	<< std::endl;
	this->sendChannelMsg(":" + newClient->getNickName() + "!~" + newClient->getIdentName() + "@" + newClient->getIpHostName() + " JOIN " + this->name + "\r\n");
	this->sendTopic(newUser.client);
	this->sendNames(newUser.client);
	std::cout	<< C_LGREEN	<< "User "
				<< C_RESET	<< newClient->getNickName()
				<< C_LGREEN	<< " joined "
				<< C_RESET	<< this->name	<< std::endl;
	this->sendWho(newUser.client);
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

	msg = ": 332 " + client->getNickName() + " " + this->name + " :" + this->topic + "\r\n";
	client->sendMsg(msg);
}

void	Channel::sendNames(Client *client)
{
	std::string	msg;

	msg = ": 353 " + client->getNickName() + " = " + this->name + " :";
	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
	{
		if ((*i).admin == true)
			msg += "@";
		msg += (*i).client->getNickName() + " ";
	}
	client->sendMsg(msg + "\r\n");
	msg = ": 366 " + client->getNickName() + " " + this->name + " :end of /NAMES list.\r\n";
	client->sendMsg(msg);
}

void	Channel::sendChannelMsg(const std::string msg) const
{
	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
		(*i).client->sendMsg(msg);
}

void	Channel::sendPrivMsg(Client *sender, std::string msg)
{
	msg = ":" + sender->getNickName() + " PRIVMSG " + this->name + " :" + msg + "\r\n";
	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
		if ((*i).client != sender)
			(*i).client->sendMsg(msg);
}

void	Channel::sendWho(Client *client)
{
	std::string	msg;

	msg = ": 352 " + client->getNickName() + " " + this->name + " ";
	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
	{
		std::string	msgWho;
		msgWho = msg +	(*i).client->getIdentName() + " " + \
						(*i).client->getIpHostName() + " " + \
						(*i).client->getServer() + " " + \
						(*i).client->getNickName();
		if ((*i).admin == true)
			msgWho += " H@:0 ";
		else
			msgWho += " H:0 ";
		msgWho +=		(*i).client->getRealName() + "\r\n";
		client->sendMsg(msgWho);
	}
	msg = ": 315 " + client->getNickName() + " " + this->name + " :End of /WHO list.\r\n";
	client->sendMsg(msg);
}

// void	Channel::sendWhoIs(Client *client, Client *who)
// {

// }

void	Channel::inviteClient(Client *client)
{
	std::string	msg;

	msg = client->getNickName() + "!" + client->getUserName() + "JOIN" + this->name + "\r\n";
	client->sendMsg(msg);
}

void	Channel::removeUser(const Client *client)
{
	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
		if ((*i).client == client)
		{
			this->users.erase(i);
			this->sendChannelMsg(":" + client->getNickName() + "!" + client->getIdentName() + "@" + client->getIpHostName() + " QUIT\r\n");
			std::cout	<< C_PURPLE	<< "triggered"	<< C_RESET	<< std::endl; // NEEDS TESTING!
			return ;
		}
}

// void	Channel::kickClient(Client *client)
// {
// 	for (size_t i = this->users.size(); i > 0; --i)
// 	{
// 		if (this->users[i - 1].client == client)
// 		{
// 			this->users[i - 1].client->sendMsg("PART " + this->name);
// 			this->users.erase(this->users.begin() + i - 1);
// 			return ;
// 		}
// 	}
// }

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
