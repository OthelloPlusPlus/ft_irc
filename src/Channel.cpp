/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/09/13 19:08:49 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "verboseCheck.hpp"
#include "colors.hpp"

#include <iostream>
// std::
#include <chrono>
// std::chrono

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

Channel::Channel(std::string name, Server *server): name(name), topic(""), server(server)
{
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

void	Channel::addClient(Client *newClient, bool admin)
{
	ChannelUser	newUser;

	if (this->userIsInChannel(newClient))
	{
		if (verboseCheck() >= V_CHANNEL)
			std::cout	<< C_LRED	<< "User "
						<< C_RESET	<< newClient->getNickName()
						<< C_LRED	" is already in channel "
						<< C_RESET	<< this->name	<< std::endl;
		return ;
	}
	newUser.client = newClient;
	newUser.admin = admin;
	newUser.timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());;
	this->users.push_back(newUser);
	this->sendToChannel(":" + newClient->getNickName() + "!~" + newClient->getIdentName() + "@" + newClient->getIpHostName() + " JOIN " + this->name + "\r\n");
	if (!this->topic.empty())
		this->sendTopic(newUser.client);
	this->sendNames(newUser.client);
	if (verboseCheck() >= V_CHANNEL)
		std::cout	<< C_LGREEN	<< "User "
					<< C_RESET	<< newClient->getNickName()
					<< C_LGREEN	<< " joined channel "
					<< C_RESET	<< this->name	<< std::endl;
	this->sendWho(newUser.client);
}

// void	Channel::inviteClient(Client *client)
// {
// 	std::string	msg;

// 	msg = client->getNickName() + "!" + client->getUserName() + "JOIN" + this->name + "\r\n";
// 	client->sendMsg(msg);
// }

void	Channel::removeUser(const Client *client)
{
	if (this->userIsInChannel(client))
	{
		this->sendToChannel(":" + client->getNickName() + "!~" + client->getIdentName() + "@" + client->getIpHostName() + " PART " + this->name + "\r\n");
		for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end();)
		{
			if ((*i).client == client)
			{
				i = this->users.erase(i);
			}
			else
				++i;
		}
		if (verboseCheck() >= V_CHANNEL)
			std::cout	<< C_LMGNT	<< "User "
						<< C_RESET	<< client->getNickName()
						<< C_LMGNT	<< " left channel "
						<< C_RESET	<< this->name	<< std::endl;
	}
}

void	Channel::promoteOldestUser(void)
{
	std::vector<ChannelUser>::iterator	oldest = this->users.begin();
	for (std::vector<ChannelUser>::iterator user = oldest; user != this->users.end(); ++user)
		if ((*user).timestamp < (*oldest).timestamp)
			oldest = user;
	(*oldest).admin = true;
	this->sendToChannel(":" + this->server->getName() + " MODE " + this->name + " +o " + (*oldest).client->getNickName() + "\r\n");
}

// void	Channel::setAdmin(Client *target, bool status)
// {
// 	for (std::vector<ChannelUser>::iterator user = this->users.begin(); user != this->users.end(); ++user)
// 		if ((*user).client == target)
// 		{
// 			if (verboseCheck() >= V_ADMIN)
// 				std::cout	<< __func__	<<__LINE__	<< std::endl;
// 			(*user).admin = status;
// 			break ;
// 		}
// }

void	Channel::setMode(Client &client, std::string flag, std::string arg)
{
	if (flag.size() > 2)
	{
		std::cout	<< "Flag Error"	<< std::endl;
		return ;
	}
	if (flag.empty())
		this->sendMode(client);
	else if (this->userIsAdmin(client))
	{
		char	marker = flag.back();
		if (marker == 'i')
			this->setModeI(client, flag);
		else
			std::cout	<< "Unknown flag "	<< flag	<< std::endl;
	}
	else 
		std::cout	<< "No priveligies"	<< std::endl;
}

void	Channel::sendMode(Client &client) const
{
	std::string	msg = ':' + this->server->getName() + " 324 " + client.getNickName() + ' ' + this->name + " \n";
	if (this->modeInvite)
		msg += "Invite(i):\t true\t Invite only channel\n";
	else
		msg += "Invite(i):\t false\t Public channel\n";
	msg += "Topic(t):\t trueish\t Unrestricted topic\n";
	msg += "Key(k):\t trueish \t Requires password\n";
	msg += "Operator(o):\t Give or take operator priveliges\n";
	msg += "\r\n";
	client.sendMsg(msg);
}

void	Channel::setModeI(Client &client, std::string flag)
{
	if (flag[0] == '-')
		this->modeInvite = false;
	else if (flag[0] == '+')
		this->modeInvite = true;
	else
		this->modeInvite = !this->modeInvite;
	std::cout	<< "I has been set to "	<< this->modeInvite	<< std::endl;
}

void	Channel::setTopic(Client &client, const std::string newTopic)
{
	if (this->topic == newTopic || \
		(this->topic.empty() && newTopic == "-"))
		return ;

	std::vector<ChannelUser>::iterator user = this->users.begin();
	for (; user != this->users.end(); ++user)
		if (&client == (*user).client)
			break ;
	if (user == this->users.end())
		return ;
	if ((*user).admin == true || this->modeTopic == false)
	{
		if (newTopic == "-")
			this->topic = "";
		else
			this->topic = newTopic;
		this->sendToChannel(":" + client.getNickName() + " TOPIC " + this->name + " :" + this->topic + "\r\n");
		if (verboseCheck() >= V_CHANNEL)
		{
			std::cout	<< C_RESET	<< "User "
						<< C_LCYAN	<< client.getNickName();
			if (this->topic.empty())
				std::cout	<< C_RESET	<< " unset channel "
							<< C_LCYAN	<< this->name
							<< C_RESET	<< "'s topic.";
			else
				std::cout	<< C_RESET	<< " changed channel "
							<< C_LCYAN	<< this->name
							<< C_RESET	<< "'s topic to "
							<< C_LCYAN	<< this->topic;
			std::cout	<< C_RESET	<< std::endl;
		}
	}
	else
	{
		if (verboseCheck() >= V_CHANNEL)
			std::cout	<< C_LRED	<< "User "
						<< C_RESET	<< client.getNickName()
						<< C_LRED	<< " tried to change channel "
						<< C_RESET	<< this->name
						<< C_LRED	<< "'s topic without appropriate permissions."
						<< C_RESET	<< std::endl;
	}
}

void	Channel::sendTopic(Client *client) const
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

void	Channel::sendWho(Client *client)
{
	std::string	msg;

	msg = ":" + this->server->getName() + " 352 " + client->getNickName() + " " + this->name + " ";
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
	msg = ":" + this->server->getName() + " 315 " + client->getNickName() + " " + this->name + " :End of /WHO list.\r\n";
	client->sendMsg(msg);
}

void	Channel::sendToChannel(const std::string msg) const
{
	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
		(*i).client->sendMsg(msg);
}

void	Channel::sendToChannel(const Client *exclude, const std::string msg) const
{
	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
		if ((*i).client != exclude)
			(*i).client->sendMsg(msg);
}

// void	Channel::sendPrivMsg(Client *sender, std::string msg)
// {
// 	msg = ":" + sender->getNickName() + " PRIVMSG " + this->name + " :" + msg + "\r\n";
// 	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
// 		if ((*i).client != sender)
// 			(*i).client->sendMsg(msg);
// }

bool	Channel::userIsInChannel(const Client *client) const
{
	for (std::vector<ChannelUser>::const_iterator i = this->users.begin(); i != this->users.end(); ++i)
		if ((*i).client == client)
			return (true);
	return (false);
}

std::string	Channel::getName(void) const
{
	return (this->name);
}

bool	Channel::userIsAdmin(const Client &client) const
{
	for (std::vector<ChannelUser>::const_iterator user = this->users.begin(); user != this->users.end(); ++user)
		if (&client == (*user).client)
			return ((*user).admin);
	return (false);
}

// std::string	Channel::getTopic(void) const
// {
// 	return (this->topic);
// }

size_t	Channel::getSize(void) const
{
	return (this->users.size());
}

size_t	Channel::getAdminSize(void) const
{
	size_t	size;

	size = 0;
	for (std::vector<ChannelUser>::const_iterator user = this->users.begin(); user != this->users.end(); ++user)
		if ((*user).admin == true)
			++size;
	return (size);
}

// void	Channel::printClientList(void) const
// {
// 	std::cout	<< "--- Channel "	<< this->name	<< ":"	<< std::endl;
// 	for (std::vector<ChannelUser>::const_iterator client = this->users.begin(); client != this->users.end(); ++client)
// 		std::cout	<<	client->client->getNickName()	<<'t'	<< (*client).admin	<< '\n';
// 	std::cout	<< "--- End of list\n"	<< std::endl;
// }



























// // void	Channel::kickClient(Client *client)
// // {
// // 	for (size_t i = this->users.size(); i > 0; --i)
// // 	{
// // 		if (this->users[i - 1].client == client)
// // 		{
// // 			this->users[i - 1].client->sendMsg("PART " + this->name);
// // 			this->users.erase(this->users.begin() + i - 1);
// // 			return ;
// // 		}
// // 	}
// // }

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
