
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

void	ServerBot::botRespond(std::string name, AClient &src, const std::string cmd, const std::vector<std::string> &args)
{
	if (cmd == "PRIVMSG")
		this->botRespondPrivMsg(name, args);
	(void)src;
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

	if (cmd == "//help")
		this->thinkHelp(dest);
	else if (cmd == "//shutdown")
		this->thinkShutdown(dest, arg);
}

void	ServerBot::thinkHelp(std::string dest)
{
	this->send.push("PRIVMSG " + dest + " :I dont know how to help you yet!");
}

void	ServerBot::thinkShutdown(std::string dest, std::string arg)
{
	size_t	pos = arg.find(' ');
	if (pos == std::string::npos)
		this->send.push("PRIVMSG " + dest + " :You need to provide a password");
	else
	{
		AClient *client	= this->_server.getClient(dest);
		if (client == nullptr)
			return ;
		this->_server.shutdownServer(*client, arg.substr(pos + 1));
	}
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

	this->botRespond(name, std::get<0>(prsd), std::get<1>(prsd), std::get<2>(prsd));
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

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */
