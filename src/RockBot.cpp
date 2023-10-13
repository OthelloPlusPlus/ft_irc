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
#include <algorithm>
//std::transform

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

void	RockBot::funFactsFiller(){
	RockBot::funFactsRock[0] = "";
	RockBot::funFactsRock[1] = "";
	RockBot::funFactsRock[2] = "";
	RockBot::funFactsRock[3] = "";
	RockBot::funFactsRock[4] = "";
	RockBot::funFactsRock[5] = "";
	RockBot::funFactsRock[6] = "";
	RockBot::funFactsRock[7] = "";
	RockBot::funFactsRock[8] = "";
	RockBot::funFactsRock[9] = "";

	RockBot::funFactsPaper[0] = "";
	RockBot::funFactsPaper[1] = "";
	RockBot::funFactsPaper[2] = "";
	RockBot::funFactsPaper[3] = "";
	RockBot::funFactsPaper[4] = "";
	RockBot::funFactsPaper[5] = "";
	RockBot::funFactsPaper[6] = "";
	RockBot::funFactsPaper[7] = "";
	RockBot::funFactsPaper[8] = "";
	RockBot::funFactsPaper[9] = "";

	RockBot::funFactsScissors[0] = "";
	RockBot::funFactsScissors[1] = "";
	RockBot::funFactsScissors[2] = "";
	RockBot::funFactsScissors[3] = "";
	RockBot::funFactsScissors[4] = "";
	RockBot::funFactsScissors[5] = "";
	RockBot::funFactsScissors[6] = "";
	RockBot::funFactsScissors[7] = "";
	RockBot::funFactsScissors[8] = "";
	RockBot::funFactsScissors[9] = "";

}

void	RockBot::rockBotRespond(std::string name, const std::string cmd, const std::vector<std::string> &args){
	if (cmd == "INVITE")
		this->botRespondInvite(args);
	else if (cmd == "JOIN")
		this->botRespondJoin(args);
	else if (cmd == "PART")
		this->botRespondPart(args);
	else if (cmd == "PRIVMSG")
		this->botRespondPrivMsg(name, args);
}

void	RockBot::botRespondInvite(const std::vector<std::string> &args){
	Channel *channel = this->_server.getChannel(args[1]);

	if (channel == nullptr)
		return ;
	this->send.push("JOIN " + channel->getName());
}

void	RockBot::botRespondJoin(const std::vector<std::string> &args){
	Channel *channel = this->_server.getChannel(args[0]);

	if (channel == nullptr)
		return ;
	this->send.push("PRIVMSG " + channel->getName() + " :I'm a bot with a hand! Look //throw a shape...rock, paper or scissors?");
}

void	RockBot::botRespondPart(const std::vector<std::string> &args)
{
	Channel *channel = this->_server.getChannel(args[0]);

	if (channel != nullptr && channel->getSize() <= 1)
		this->send.push("PART " + channel->getName());
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
			dest = client->getNickName();
	}
	if (!dest.empty())
		this->think(dest, args[1]);
}

void	RockBot::think(std::string dest, std::string arg) {
	std::string	cmd = arg.substr(0, arg.find(' '));
	if (cmd == "//throw")
		this->thinkPlay(dest, arg);
	else if (cmd == "//restart")
		this->botResetGame(dest, arg);
	else if (cmd == "//stats")
		this->botResetGame(dest, arg);
	else if (dest[0] != '#')
		this->send.push("PRIVMSG " + dest + " :Huh?");
}

void	RockBot::thinkPlay(std::string dest, std::string arg){
	try 
	{
		hand_t	hand = this->findGame(dest);
		if (this->getPlayerMove(hand, arg))
			this->rockMove(dest, hand);
		this->updateGame(dest, hand);
	}
	catch(const std::exception& e)
	{
		this->send.push("PRIVMSG " + dest + " :" + e.what());
	}
}

bool	RockBot::getPlayerMove(hand_t &hand, std::string arg){
	size_t	pos1 = arg.find(' ');
	size_t	notSpace = arg.find_first_not_of(' ', pos1);
	size_t	lastNotSpace = arg.find_last_not_of(' ');
	std::string	input;
	
	if (notSpace == std::string::npos){
		if (hand.moves == 0)
			throw(std::runtime_error("Rock, paper or scissors?"));
		else
			throw(std::runtime_error("Ok, I'm ready"));
		return (false);
	}

	input = arg.substr(notSpace, lastNotSpace - notSpace + 1);

	std::string	shape = input;
	std::transform(shape.begin(), shape.end(), shape.begin(), ::tolower);

	if (shape.compare("rock") != 0 && shape.compare("paper") != 0  && shape.compare("scissors") != 0 ){
		throw(std::runtime_error("Mmmhh...I don't recognise this shape: " + input ));
		return (false);
	}
	hand.shapes[USER] = shape;
	hand.moves++;
	return (true);
}

//		std::cout	<< __func__ << __LINE__	<< std::endl;

e_move	mapToeMove(std::string shape){
	if (shape == "rock") return e_move::ROCK;
	if (shape == "paper") return e_move::PAPER;
	if (shape == "scissors") return e_move::SCISSORS;
	return e_move::ROCK;
}

std::string	RockBot::getFunFact(std::string shape){
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	int factIndex = srand() % ARR_SIZE;

	return ("");
}

void	RockBot::rockMove(std::string dest, hand_t &hand){
	if (hand.shapes[USER].empty())
		return ;

	std::srand(static_cast<unsigned>(std::time(nullptr)));
	int rockMoveIndex = std::rand() % 3;
	hand.shapes[BOT] = hand.options[rockMoveIndex];
	
	e_move playerMove = mapToeMove(hand.shapes[USER]);
	e_move botMove = mapToeMove(hand.shapes[BOT]);

	this->send.push("PRIVMSG " + dest + " :You play " + hand.shapes[USER] + " and I reply " + hand.shapes[BOT]);
	if (playerMove == botMove){
		this->send.push("PRIVMSG " + dest + " :We tied!");
		return ;
	}

	if ((playerMove == e_move::ROCK && botMove == e_move::SCISSORS)	|| \
		(playerMove == e_move::PAPER && botMove == e_move::ROCK)	|| \
		(playerMove == e_move::SCISSORS  && botMove == e_move::PAPER)){
		this->send.push("PRIVMSG " + dest + " :You won!");
		hand.winner[USER] += 1;
	}
	else {
		this->send.push("PRIVMSG " + dest + " :I won!");
		hand.winner[BOT] += 1;
	}
	return ;
}

void	RockBot::botResetGame(std::string dest, std::string arg){
	std::map<std::string, hand_t>::const_iterator i = this->hand.find(dest);
	if (i != this->hand.end()){
		this->hand.erase(i);
		this->newGame(dest);
		i = this->hand.find(dest);
	}
	this->send.push("PRIVMSG " + dest + " :Game reset. Let's start over");
}

void	RockBot::botShowStats(std::string dest, std::string arg){
	hand_t	showGame =  this->findGame(dest);
	this->send.push("PRIVMSG " + dest + " :Game level = " + std::to_string(showGame.level));
	this->send.push("PRIVMSG " + dest + " :Game total moves = " + std::to_string(showGame.moves));
	this->send.push("PRIVMSG " + dest + " :Game score = " + dest + " = " + std::to_string(showGame.winner[USER]) \
														+ " - RockBot = " + std::to_string(showGame.winner[BOT]));
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

	newGame.level = 1;
	newGame.moves = 0;
	newGame.winner[USER] = 0;
	newGame.winner[BOT] = 0;
	newGame.options[0] = "rock";
	newGame.options[1] = "paper";
	newGame.options[2] = "scissors";
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
		this->rockBotRespond(name, std::get<1>(parsed), std::get<2>(parsed));
		this->recv.pop();
	}
	if (!send.empty()){
		reply = this->send.front();
		this->send.pop();
	}
	if (!reply.empty())
		reply += "\r\n";
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