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
	this->send.push("PRIVMSG " + channel->getName() + " :Let's play! Rock, paper or scissors?\r\n");
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
		this->think(dest, args[1]);
		// this->send.push("PRIVMSG " + dest + " :Huh?\r\n");
}

void	RockBot::think(std::string dest, std::string arg) {
	std::string	cmd = arg.substr(0, arg.find(' '));

	if (cmd == "//play")
		this->thinkPlay(dest, arg);
	else if (dest[0] != '#')
		this->send.push("PRIVMSG " + dest + " :Huh?\r\n");
}

void	RockBot::thinkPlay(std::string dest, std::string arg){
	try 
	{
		hand_t	hand = this->findGame(dest);

		if (this->getPlayerMove(hand, arg))
			this->send.push("PRIVMSG " + dest + " :test?\r\n");
		this->updateGame(dest, hand);
		// 	this->counterMove(game); ++game.moves
		// this->sendGame(dest, game);
		// this->updateGame(dest, game);
	}
	catch(const std::exception& e)
	{
		this->send.push("PRIVMSG " + dest + " :" + e.what() + "\r\n");
	}
}

bool	RockBot::getPlayerMove(hand_t &hand, std::string arg){
	size_t	pos1 = arg.find(' ');
	size_t	pos2 = arg.find(' ', pos1 + 1);
	std::string	shape;

	std::cout << C_BLUE "arg [" << arg << "]" C_RESET << std::endl;
	if (pos1 == pos2){
		if (hand.moves == 0){
			throw(std::runtime_error("Rock, paper or scissors?"));
			return (false);
		}
	}
	// try
	// {
		std::cout	<< __func__ << __LINE__	<< std::endl;
		shape = arg.substr(pos1 + 1, pos2 + 1);
		std::cout	<< __func__ << __LINE__	<< std::endl;
		std::cout << C_AZURE "shape [" << shape << "]" C_RESET << std::endl;
		if (shape.empty()){
			std::cout	<< __func__ << __LINE__	<< std::endl;
			throw(std::runtime_error("Rock, paper or scissors?"));
			return (false);
		}
		else {
			std::cout	<< __func__ << __LINE__	<< std::endl;
			size_t	notSpace = shape.find_first_not_of(' ');
			std::cout	<< __func__ << __LINE__	<< std::endl;
			if (notSpace != std::string::npos) {
				std::cout << "not empty" << std::endl;
				std::cout	<< __func__ << __LINE__	<< std::endl;
				throw(std::runtime_error("Not empty"));
				std::cout	<< __func__ << __LINE__	<< std::endl;
				return (true);
				std::cout	<< __func__ << __LINE__	<< std::endl;
			}
			else {
				std::cout	<< __func__ << __LINE__	<< std::endl;
				std::cout << "only spaces" <<std::endl;
				std::cout	<< __func__ << __LINE__	<< std::endl;
				throw(std::runtime_error("Huh?"));
				std::cout	<< __func__ << __LINE__	<< std::endl;
				return (false);
				std::cout	<< __func__ << __LINE__	<< std::endl;
			}
		}
	// }
	// catch(const std::exception &e)
	// {
	// 	std::cout	<< __func__ << __LINE__	<< std::endl;
	// 	throw(std::runtime_error("I don't recognise command: " + arg.substr(pos1 + 1, pos2 + 1)));
	// }
	// std::cout	<< __func__ << __LINE__	<< std::endl;
	return (true);
}


hand_t	RockBot::findGame(std::string key) {
	std::map<std::string, hand_t>::const_iterator i = this->hand.find(key);
	if (i == this->hand.end())
	{
		this->newGame(key);
		i = this->hand.find(key);
	}
	return (i->second);
}

void	RockBot::newGame(std::string key){
	hand_t	newGame;

	// newGame.level = 1;
	// this->clearGame(newGame);
	this->updateGame(key, newGame);
}

void	RockBot::updateGame(std::string key, hand_t &update)
{
	this->hand[key] = update;
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