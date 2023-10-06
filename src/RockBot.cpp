#include "RockPaperScissors.hpp"
#include "Command.hpp"
#include "colors.hpp"
#include "Parse.hpp"

#include <iostream>
// std::
#include <cstdlib>
// int rand();
#include <ctime>
// std::time_t time( std::time_t* arg );

/****************************************************************************************\
*			Constructor 																 *
\****************************************************************************************/

RockBot::RockBot(Server &server): AClient (server) {
	this->_nickName = "RockBot";
	this->_userName = "RockPaperScissorBot";
	this->_realName = "Rock-paper-scissors playing Bot";
	this->_isRegistered = true;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	std::cout	<< C_DGREEN	<< "Default constructor "
				<< C_GREEN	<< "RockBot"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

/****************************************************************************************\
*			Destructor 																 	 *
\****************************************************************************************/

RockBot::~RockBot(void) {
	this->closeFD();
	std::cout	<< C_DRED	<< "Deconstructor "
				<< C_RED	<< "RockBot"
				<< C_DRED	<< " called"
				<< C_RESET	<< std::endl;
}

void	RockBot::rockBotRespond(std::string name, AClient &src, const std::string cmd, const std::vector<std::string> &args){
	if (cmd == "INVITE")
		this->botRespondInvite(args);
	else if (cmd == "JOIN")
		this->botRespondJoin(args);
	// else if (cmd == "PART")
	// 	this->botRespondPart(args);
	else if (cmd == "PRIVMSG")
		this->botRespondPrivMsg(name, args);
}

void	RockBot::botRespondInvite(const std::vector<std::string> &args){
	Channel *channel = this->_server.getChannel(args[1]);

	if (channel == nullptr)
		return ;
	this->send.push("JOIN " + channel->getName() + "\r\n");
}

void	RockBot::botRespondJoin(const std::vector<std::string> &args){
	Channel *channel = this->_server.getChannel(args[0]);

	if (channel == nullptr)
		return ;
	this->send.push("PRIVMSG " + channel->getName() + " :Rock, paper or scissors?\r\n");
}

void	RockBot::botRespondPart(const std::vector<std::string> &args)
{
	Channel *channel = this->_server.getChannel(args[0]);

	if (channel != nullptr && channel->getSize() <= 1)
		this->send.push("PART " + channel->getName() + "\r\n");
}

void	RockBot::botRespondPrivMsg(std::string name, const std::vector<std::string> &args) {
	std::string	dest;

	if (args[0][0] == '#')
	{
		Channel *channel = this->_server.getChannel(args[0]);
		if (channel != nullptr)
			dest = channel->getName();
	}
	else
	{
		AClient *client = this->_server.getClient(name);
		if (client != nullptr)
			dest = client->getBestName();
	}
	if (!dest.empty())
	// this->think(dest, args[1]);
		this->send.push("PRIVMSG " + dest + " :Huh?\r\n");
}

std::string	RockBot::getMsg(void) {
	std::string reply;

	while (!this->recv.empty()) {
		std::string name = this->recv.front().substr(1, this->recv.front().find('!') - 1);
		std::string args = this->recv.front().substr(this->recv.front().find(' ') + 1);

		std::tuple<AClient &, std::string, std::vector<std::string>> parsed = Parse::parseMsg(*this, args);
		this->rockBotRespond(name, std::get<0>(parsed), std::get<1>(parsed), std::get<2>(parsed));
		this->recv.pop();
	}
	if (!send.empty()){
		reply = this->send.front();
		this->send.pop();
	}
	return (reply);
}

void	RockBot::sendMsg(std::string msg) {
	this->recv.push(msg);
}

bool	RockBot::stillActive(void) const {
	return (true);
}

void	RockBot::closeFD(void) {
	while (!this->recv.empty())
		this->recv.pop();
	while (!this->send.empty())
		this->send.pop();
}