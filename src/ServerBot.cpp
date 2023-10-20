/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerBot.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/19 20:24:51 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/10/19 20:39:59 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBot.hpp"
#include "verboseCheck.hpp"
#include "Parse.hpp"
#include "colors.hpp"

#include <tuple>
// std::tuple

/** ************************************************************************ **\
 * 
 * 	Constructors
 * 
\* ************************************************************************** */

ServerBot::ServerBot(Server &server): AClient(server)
{
	this->_nickName = "ServerBot";
	this->_userName = "ServerBot";
	this->_realName = "Server system management and user help bot";
	this->_isRegistered = true;
	this->setIsOperator(true);
	if (verboseCheck() >= V_DETAILS)
		std::cout	<< C_GREEN	<< this->_nickName
					<< C_DGREEN	<< ": Server gathers, and now my watch begins."
					<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

ServerBot::~ServerBot(void)
{
	this->closeFD();
	if (verboseCheck() >= V_DETAILS)
		std::cout	<< C_RED	<< this->_nickName
					<< C_DRED	<< ": And now my watch has end."
					<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

void	ServerBot::botRespond(std::string name, const std::string cmd, const std::vector<std::string> &args)
{
	if (cmd == "PRIVMSG")
		this->botRespondPrivMsg(name, args);
}

void	ServerBot::botRespondPrivMsg(std::string name, const std::vector<std::string> &args)
{
	if (args[0][0] == '#')
		this->send.push("PART " + args[0] + " :I don't do channels");
	else
	{
		AClient *client = this->_server.getClient(name);
		if (client == nullptr)
			return ;
		this->think(client->getNickName(), args[1]);
	}
}

void	ServerBot::think(std::string dest, std::string arg)
{
	std::string	cmd = arg.substr(0, arg.find(' '));

	if (cmd == "//info")
		this->thinkInfo(dest);
	else if (cmd == "//motd")
		this->thinkMOTD(dest, arg);
	else if (cmd == "//shutdown")
		this->thinkShutdown(dest, arg);
	else if (cmd == "//help")
		this->thinkHelp(dest);
	else
		this->send.push("PRIVMSG " + dest + " :I am sorry, I do not know what that means.");
}

void	ServerBot::thinkInfo(std::string dest)
{
	std::string	msg("PRIVMSG " + dest);

	this->send.push(msg + " :Server:\t" + this->_server.getName());
	this->send.push(msg + " :IP:\t" + this->_server.getIP());
	this->send.push(msg + " :Port:\t" + std::to_string(this->_server.getPort()));
	this->send.push(msg + " :MotD:\t" + this->_server.getMOTD());
	this->send.push(msg + " :Clients:\t" + std::to_string(this->_server.getClientsSize()));
	this->send.push(msg + " :Channels:\t" + std::to_string(this->_server.getChannelsSize()));
	std::string oper;
	AClient *client = this->_server.getClient(dest);
	if (client != nullptr && client->getIsOperator())
			oper = "(operator)";
	this->send.push("PRIVMSG " + dest + " :User:\t" + dest + oper);
}

void	ServerBot::thinkShutdown(std::string dest, std::string arg)
{
	size_t	pos = arg.find(' ');

	if (pos == std::string::npos)
		this->send.push("PRIVMSG " + dest + " :You need to provide a password.");
	else
	{
		AClient *client	= this->_server.getClient(dest);
		if (client == nullptr)
			return ;
		this->_server.shutdownServer(*client, arg.substr(pos + 1));
		this->_server.sendAllUsers(':' + this->_nickName + " PRIVMSG ", *client, " :Server Shutdown!");
	}
}

void	ServerBot::thinkMOTD(std::string dest, std::string arg)
{
	size_t	pos = arg.find(' ');

	if (pos != std::string::npos)
	{
		AClient *client = this->_server.getClient(dest);
		if (client != nullptr)
			this->_server.setMOTD(*client, arg.substr(pos + 1));
	}
	this->send.push("PRIVMSG " + dest + " :MotD: " + this->_server.getMOTD());
}

void	ServerBot::thinkHelp(std::string dest)
{
	this->send.push("PRIVMSG " + dest + " :Hello, I am ServerBot! I am part of this server and can help you a bit.");
	this->send.push("PRIVMSG " + dest + " :Right now I can:");
	this->send.push("PRIVMSG " + dest + " ://info:\tTo provide you with server information");
	this->send.push("PRIVMSG " + dest + " ://motd [new message]:\tTo change the Message of the Day");
	this->send.push("PRIVMSG " + dest + " ://shutdown [password]:\tTo shutdown the server");
	this->send.push("PRIVMSG " + dest + " :You can also:");
	this->send.push("PRIVMSG " + dest + " :/join [channel]:\tTo join a channel");
	this->send.push("PRIVMSG " + dest + " :/part [channel]:\tTo leave a channel");
	this->send.push("PRIVMSG " + dest + " :/msg [user] [message]:\tTo message a user");
}

bool	ServerBot::stillActive(void) const
{
	return (true);
}

void	ServerBot::sendMsg(std::string msg)
{
	std::string	name(msg.substr(1, msg.find('!') - 1));
	std::string args(msg.substr(msg.find(' ') + 1));
	std::tuple<AClient &, std::string, std::vector<std::string> >\
				prsd(Parse::parseMsg(*this, args));

	this->botRespond(name, std::get<1>(prsd), std::get<2>(prsd));
}

std::string	ServerBot::getMsg(void)
{
	std::string	reply;

	if (!this->send.empty())
	{
		reply = this->send.front() + "\r\n";
		this->send.pop();
	}
	return (reply);
}

void	ServerBot::closeFD(void)
{
	while (!this->send.empty())
		this->send.pop();
}
