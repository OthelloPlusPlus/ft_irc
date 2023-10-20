/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RockBot.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/20 18:18:15 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/20 18:46:06 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "RockBot.hpp"
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


std::string RockBot::funFactsRock[] = {
	"Magma that comes from volcanoes is liquid rock.",
	"Some rocks can float on water.",
	"Diamonds, the hardest known material on Earth, are a 10 on the Mohs scale.",
	"The colors held inside some rocks and minerals have been used by artists for thousands of years.",
	"Rocks, stones, and minerals are different things.",
	"Rocks are made up of minerals, while minerals are made up of chemicals found in the earth.",
	"Several rocks around the world are so culturally important that they symbolize a whole city, region, or civilization.",
	"The Most Famous Rock is the Rosetta Stone.",
	"Metamorphic rock has been put under a lot of pressure and heat.",
	"Geology is the study of the rocks."
	};

std::string RockBot::funFactsPaper[] = {
	"Chinese were the first nation in the world who started producing paper by hand, 2000 years ago.",
	"Oldest surviving trace of papyrus comes from 3000 BC Egypt.",
	"Machine that extracts and prepares tree fibers for papermaking is called Hollander.",
	"Recycling one ton of paper saves 17 trees!",
	"Toilet paper started being produced in late 9th century China.",
	"Watermarks on paper were first used in 13th century Italy.",
	"The oldest European document written on paper comes from 1109. It was a deed of Sicillian king King Roger II.",
	"First book that was printed from an industrially made paper was produced in 1804. ",
	"The Chinese are credited with being the first people to use paper currency and toilet paper.",
	"It can take five liters of water to make one sheet of paper."
	};

std::string RockBot::funFactsScissors[] = {		
	"You shouldn't run with scissors.",
	"The first scissors with a pivot between the blades were being used by the sixth century AD.",
	"The first scissor blades were constructed from bronze.",
	"Modern scissors are generally made with stainless steel blades.",
	"Scissors larger in size than approximately six or seven inches are called shears.",
	"Japanese scissors have very sharp blades with honed edges that taper to a sharp point.",
	"It is estimated that scissors were invented in 1500 BC in Egypt, modified later by the Romans in 100 AD.",
	"The world’s largest pair of scissors was presented 2009. Made of steel, they weighed fifty-five pounds and measured more than seven feet long. They were fully functional!",
	"Popular myth has their inventor being Leonardo Da Vinci; however they actually pre-date Da Vinci as they were in use in ancient times.",
	"The term ‘scissors’ comes from the Latin word ‘cisoria’, meaning ‘cutting instrument’."
	};

/****************************************************************************************\
*			Constructor																	*
\****************************************************************************************/

RockBot::RockBot(Server &server): AClient (server){

	this->_nickName = "RockBot";
	this->_userName = "RockyBot";
	this->_realName = "Rock-paper-scissors Bot";
	this->_isRegistered = true;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	std::cout	<< C_GREEN	<< "RockBot "
				<< C_DGREEN	<< "Did you summon me?"
				<< C_GREEN	<< "RockBot "
				<< C_DGREEN	<< "is here."
				<< C_RESET	<< std::endl;
}

/****************************************************************************************\
*			Destructor																	*
\****************************************************************************************/

RockBot::~RockBot(void){

	this->closeFD();
	if (!this->hand.empty())
		this->hand.clear();
	std::cout	<< C_RED	<< "RockBot"
				<< C_DRED	<< " says: I'll be back"
				<< C_RESET	<< std::endl;
}

void	RockBot::rockBotRespond(std::string name, const std::string cmd, const std::vector<std::string> &args){

	if (cmd == "INVITE")
		this->botRespondInvite(args);
	else if (cmd == "JOIN")
		this->botRespondJoin(name, args);
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

void	RockBot::botRespondJoin(std::string name, const std::vector<std::string> &args){

	Channel *channel = this->_server.getChannel(args[0]);

	if (channel == nullptr || name != this->_nickName)
		return ;
	this->send.push("PRIVMSG " + channel->getName() + " :I'm a bot with a hand!Look, //throw rock or paper or scissors");
}

void	RockBot::botRespondPart(const std::vector<std::string> &args){

	Channel *channel = this->_server.getChannel(args[0]);

	if (channel != nullptr && channel->getSize() <= 1)
		this->send.push("PART " + channel->getName());
}

void	RockBot::botRespondPrivMsg(std::string name, const std::vector<std::string> &args){

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

void	RockBot::think(std::string dest, std::string arg){

	std::string	cmd = arg.substr(0, arg.find(' '));
	if (cmd == "//throw")
		this->botPlays(dest, arg);
	else if (cmd == "//restart")
		this->botRestartGame(dest);
	else if (cmd == "//stats")
		this->botShowStats(dest);
	else if (cmd == "//helpbot")
		this->helpBot(dest);
	else if (dest[0] != '#')
		this->send.push("PRIVMSG " + dest + " :Huh?");
	else if (dest[0] == '#' && cmd.compare(0, 2, "//") == 0)
		this->send.push("PRIVMSG " + dest + " :Hemmhh...I don't recognise this command ¯\\_(ツ)_/¯");
}

void	RockBot::botPlays(std::string dest, std::string arg){

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


e_move	mapToeMove(std::string shape){

	if (shape == "rock") return e_move::ROCK;
	if (shape == "paper") return e_move::PAPER;
	if (shape == "scissors") return e_move::SCISSORS;
	return e_move::ROCK;
}

std::string	RockBot::getFunFact(std::string shape){

	size_t factIndex;
	size_t rockSize = sizeof(RockBot::funFactsRock) / sizeof(RockBot::funFactsRock[0]);
	size_t paperSize = sizeof(RockBot::funFactsPaper) / sizeof(RockBot::funFactsPaper[0]);
	size_t scissorSize = sizeof(RockBot::funFactsScissors) / sizeof(RockBot::funFactsScissors[0]);

	std::srand(static_cast<unsigned>(std::time(nullptr)));
	if (shape == "rock"){
		factIndex = std::rand() % rockSize;
		return (funFactsRock[factIndex]);
	}
	else if (shape == "paper"){
		factIndex = std::rand() % paperSize;
		return (funFactsPaper[factIndex]);
	}
	else if (shape == "scissors"){
		factIndex = std::rand() % scissorSize;
		return (funFactsScissors[factIndex]);
	}
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

	this->send.push("PRIVMSG " + dest + " :You play " + hand.shapes[USER] + " and I play " + hand.shapes[BOT]);
	if (playerMove == botMove){
		this->send.push("PRIVMSG " + dest + " :We tied!\t(•_•)");
		return ;
	}

	if ((playerMove == e_move::ROCK && botMove == e_move::SCISSORS)	|| \
		(playerMove == e_move::PAPER && botMove == e_move::ROCK)	|| \
		(playerMove == e_move::SCISSORS  && botMove == e_move::PAPER)){
		this->send.push("PRIVMSG " + dest + " :You won!\t(╥﹏╥)");
		hand.winner[USER] += 1;
		std::cout << "fun fact " << getFunFact(hand.shapes[USER]) << std::endl;
		this->send.push("PRIVMSG " + dest + " :Did you know? " + getFunFact(hand.shapes[USER]));
	}
	else {
		this->send.push("PRIVMSG " + dest + " :I won!\t٩(^‿^)۶");
		hand.winner[BOT] += 1;
	}
	return ;
}

void	RockBot::botRestartGame(std::string dest){

	std::map<std::string, hand_t>::const_iterator i = this->hand.find(dest);
	if (i != this->hand.end()){
		this->hand.erase(i);
		this->newGame(dest);
		i = this->hand.find(dest);
	}
	this->send.push("PRIVMSG " + dest + " :Game reset. Let's start over");
}

void	RockBot::botShowStats(std::string dest){

	hand_t	showGame =  this->findGame(dest);
	this->send.push("PRIVMSG " + dest + " :Game total moves = " + std::to_string(showGame.moves));
	this->send.push("PRIVMSG " + dest + " :Game score = " + dest + " = " + std::to_string(showGame.winner[USER]) \
														+ " vs RockBot = " + std::to_string(showGame.winner[BOT]));
}

void	RockBot::helpBot(std::string dest){

	this->send.push("PRIVMSG " + dest + " :I can play Rock - paper - scissors!");
	this->send.push("PRIVMSG " + dest + " ://throw  <shape>\tto challenge me");
	this->send.push("PRIVMSG " + dest + " ://stats\t\t\t\tto display the current game");
	this->send.push("PRIVMSG " + dest + " ://restart\t\t\tto start again");
}



hand_t	RockBot::findGame(std::string key){

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

	newGame.moves = 0;
	newGame.winner[USER] = 0;
	newGame.winner[BOT] = 0;
	newGame.options[0] = "rock";
	newGame.options[1] = "paper";
	newGame.options[2] = "scissors";
	this->updateGame(key, newGame);
}

void	RockBot::updateGame(std::string key, hand_t &update){

	this->hand[key] = update;
}

std::string	RockBot::getMsg(void){

	std::string reply;

	while (!this->recv.empty()) {
		std::string name = this->recv.front().substr(1, this->recv.front().find('!') - 1);
		std::string args = this->recv.front().substr(this->recv.front().find(' ') + 1);

		std::tuple<AClient &, std::string, std::vector<std::string> > parsed = Parse::parseMsg(*this, args);
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

void	RockBot::sendMsg(std::string msg){

	this->recv.push(msg);
}

bool	RockBot::stillActive(void) const{

	return (true);
}

void	RockBot::closeFD(void){

	while (!this->recv.empty())
		this->recv.pop();
	while (!this->send.empty())
		this->send.pop();
}

