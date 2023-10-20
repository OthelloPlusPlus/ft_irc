/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   BotTicTacToe.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/10/19 14:54:07 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "BotTicTacToe.hpp"
#include "Parse.hpp"
#include "colors.hpp"

#include <iostream>
// std::
#include <tuple>
// std::tuple
#include <cstdlib>
// int rand();
#include <ctime>
// std::time_t time( std::time_t* arg );

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
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	if (verboseCheck() >= V_DETAILS)
		std::cout	<< C_GREEN	<< this->_nickName
					<< C_DGREEN	<< ": Bot ready to play "
					<< C_GREEN	<< "Tic Tac Toe!"
					<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

BotTicTacToe::~BotTicTacToe(void)
{
	this->closeFD();
	if (verboseCheck() >= V_DETAILS)
		std::cout	<< C_RED	<< "BotTicTacToe"
					<< C_DRED	<< ": No more play?"
					<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

void	BotTicTacToe::botRespond(std::string name, const std::string cmd, const std::vector<std::string> &args)
{
	if (cmd == "INVITE")
		this->botRespondInvite(args);
	else if (cmd == "JOIN")
		this->botRespondJoin(name, args);
	else if (cmd == "PART")
		this->botRespondPart(args);
	else if (cmd == "PRIVMSG")
		this->botRespondPrivMsg(name, args);
}

void	BotTicTacToe::botRespondInvite(const std::vector<std::string> &args)
{
	Channel *channel = this->_server.getChannel(args[1]);

	if (channel == nullptr)
		return ;
	this->send.push("JOIN " + channel->getName());
}

void	BotTicTacToe::botRespondJoin(std::string name, const std::vector<std::string> &args)
{
	Channel *channel = this->_server.getChannel(args[0]);

	if (channel == nullptr || name != this->_nickName)
		return ;
	this->send.push("PRIVMSG " + channel->getName() + " :Anyone want to //play a game?");
}

void	BotTicTacToe::botRespondPart(const std::vector<std::string> &args)
{
	Channel *channel = this->_server.getChannel(args[0]);

	if (channel != nullptr && channel->getSize() <= 1)
		this->send.push("PART " + channel->getName());
}

void	BotTicTacToe::botRespondPrivMsg(std::string name, const std::vector<std::string> &args)
{
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

void	BotTicTacToe::think(std::string dest, std::string arg)
{
	std::string	cmd = arg.substr(0, arg.find(' '));

	if (cmd == "//play")
		this->thinkPlay(dest, arg);
	else if (cmd == "//level")
		this->thinkLevel(dest, arg);
	else if (cmd == "//show")
		this->thinkShow(dest);
	else if (cmd == "//reset")
		this->thinkReset(dest);
	else if (cmd == "//help")
		this->thinkHelp(dest);
	else if (dest[0] != '#')
		this->send.push("PRIVMSG " + dest + " :Huh?");
}

void	BotTicTacToe::thinkLevel(std::string dest, std::string arg)
{
	size_t	pos1 = arg.find(' ');
	size_t	pos2 = arg.find(' ', pos1 + 1);
	game_t	game = findGame(dest);

	if (pos1 != pos2)
	{
		try
		{
			int	level = std::stoi(arg.substr(pos1 + 1, pos2));
			if (level >= 0 && level <= 5)
				game.level = level;
			else
				throw (std::range_error(""));
			this->updateGame(dest, game);
		}
		catch(const std::exception& e)
		{
			this->send.push("PRIVMSG " + dest + " :Can't set level to " + arg.substr(pos1 + 1, pos2));
			return ;
		}
	}
	this->send.push("PRIVMSG " + dest + " :Level: " + std::to_string(game.level));
}

void	BotTicTacToe::thinkShow(std::string dest)
{
	this->thinkLevel(dest, "");
	this->sendGame(dest);
}

void	BotTicTacToe::thinkReset(std::string dest)
{
	game_t	game = this->findGame(dest);

	this->clearGame(game);
	this->updateGame(dest, game);
	this->send.push("PRIVMSG " + dest + " :I've reset the game");
}

void	BotTicTacToe::thinkHelp(std::string dest)
{
	this->send.push("PRIVMSG " + dest + " :I can play Tic Tac Toe!");
	this->send.push("PRIVMSG " + dest + " ://play 1-9\tto make a move");
	this->send.push("PRIVMSG " + dest + " ://level 1-5\tto set my level");
	this->send.push("PRIVMSG " + dest + " ://show \tto display the current game");
	this->send.push("PRIVMSG " + dest + " ://reset \tto reset the current game");
}

void	BotTicTacToe::thinkPlay(std::string dest, std::string arg)
{
	try 
	{
		game_t	game = this->findGame(dest);
		if (this->enterMove(game, arg))
			this->counterMove(game);
		this->sendGame(dest, game);
		this->updateGame(dest, game);
	}
	catch(const std::exception& e)
	{
		this->send.push("PRIVMSG " + dest + " :" + e.what());
	}
}

bool	BotTicTacToe::enterMove(game_t &game, std::string arg)
{
	size_t	pos1 = arg.find(' ');
	size_t	pos2 = arg.find(' ', pos1 + 1);
	int		playerPos;

	if (pos1 == pos2)
	{
		if (game.moves == 0)
			return (true);
		return (false);
	}
	try
	{
		playerPos = std::stoi(arg.substr(pos1 + 1, pos2)) - 1;
	}
	catch(const std::exception &e)
	{
		throw(std::runtime_error("I don't recognise command: " + arg.substr(pos1 + 1, pos2)));
	}
	this->fillField(game, playerPos, 'X');
	return (true);
}

void	BotTicTacToe::counterMove(game_t &game)
{
	int		spot = 0;

	if (this->gameOver(game))
		return ;
	if (game.level >= 2 && spot == 0)
		spot = this->counterMoveWin(game, -1);
	if (game.level >= 3 && spot == 0)
		spot = this->counterMoveWin(game, 1);
	if (game.level >= 4 && spot == 0)
		spot = this->counterMoveOpen(game);
	if (game.level >= 5 && spot == 0)
		spot = this->counterMoveSmart(game);
	if (spot == 0)
		spot = this->counterMoveRandom(game);
	this->fillField(game, spot - 1, 'O');
}

int	BotTicTacToe::counterMoveWin(game_t &game, int player)
{
	for (int i = 0; i < 3; ++i)
	{
		if (game.field[i][3] * player == 2)
			for (int y = 0; y < 3; ++y)
				if (game.field[i][y] == ' ')
					return (i * 3 + y + 1);
		if (game.field[3][i] * player == 2)
			for (int x = 0; x < 3; ++x)
				if (game.field[x][i] == ' ')
					return (x * 3 + i + 1);
	}
	if (game.diag[0] * player == 2)
		for (int i = 0; i < 3; ++i)
			if (game.field[i][i] == ' ')
				return (i * 4 + 1);
	if (game.diag[1] * player == 2)
		for (int i = 0; i < 3; ++i)
			if (game.field[i][2 - i] == ' ')
				return (i * 2 + 3);
	return (0);
}

int	BotTicTacToe::counterMoveOpen(game_t &game)
{
	if (game.moves == 1 && game.field[1][1] == ' ')
		return (5);
	if (game.moves <= 1)
	{
		int rand = std::rand() % 4;
		if (rand >= 2)
			++rand;
		return (rand * 2 + 1);
	}
	return (0);
}

int BotTicTacToe::counterMoveSmart(game_t &game)
{
	int	pos[3][3];
	int	current = 9;

	for (int x = 0; x < 3; ++x)//calculate values
		for (int y = 0; y < 3; ++y)
		{
			if (game.field[x][y] == ' ')
				pos[x][y] = game.field[x][3] + game.field[3][y];
			else
				pos[x][y] = 9;
			if (pos[x][y] < current)
				current = pos[x][y];
		}
	for (int x = 0; x < 3; ++x)//set lowest value to true
		for (int y = 0; y < 3; ++y)
			if (pos[x][y] == current)
				pos[x][y] = true;
			else
				pos[x][y] = false;
	for (int i = 0; i < 9; i = i + 2)//prioritise corners and middle
		if (pos[i / 3][i % 3] == true)
			return (i + 1);
	for (int i = 1; i < 9; i = i + 2)//check remaining fields
		if (pos[i / 3][i % 3] == true)
			return (i + 1);
	return (0);
}

int	BotTicTacToe::counterMoveRandom(game_t &game)
{
	int	moves = rand() % (9 - game.moves);
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			if (game.field[x][y] == ' ')
			{
				if (moves == 0)
					return (x * 3 + y + 1);
				else
					--moves;
			}
	return (0);
}

bool	BotTicTacToe::gameOver(game_t &game)
{
	game.winner = 'C';
	if (game.moves >= 9)
		game.winner = 'T';
	for (int i = 0; i < 3; ++i)
	{
		if (std::abs(game.field[3][i]) == 3)
		{
			game.winner = game.field[0][i];
			break ;
		}
		else if (std::abs(game.field[i][3]) == 3)
		{
			game.winner = game.field[i][0];
			break ;
		}
	}
	if (std::abs(game.diag[0]) == 3 || std::abs(game.diag[1]) == 3)
		game.winner = game.field[1][1];
	return (game.winner != 'C');
}

game_t	BotTicTacToe::findGame(std::string key)
{
	std::map<std::string, game_t>::const_iterator i = this->game.find(key);
	if (i == this->game.end())
	{
		this->newGame(key);
		i = this->game.find(key);
	}
	return (i->second);
}

void	BotTicTacToe::newGame(std::string key)
{
	game_t	newGame;

	newGame.level = 1;
	this->clearGame(newGame);
	this->updateGame(key, newGame);
}

void	BotTicTacToe::clearGame(game_t &game)
{
	game.field[0][0] = ' ';
	game.field[0][1] = ' ';
	game.field[0][2] = ' ';
	game.field[0][3] = 0;
	game.field[1][0] = ' ';
	game.field[1][1] = ' ';
	game.field[1][2] = ' ';
	game.field[1][3] = 0;
	game.field[2][0] = ' ';
	game.field[2][1] = ' ';
	game.field[2][2] = ' ';
	game.field[2][3] = 0;
	game.field[3][0] = 0;
	game.field[3][1] = 0;
	game.field[3][2] = 0;
	game.field[3][3] = 0;
	game.diag[0] = 0;
	game.diag[1] = 0;
	game.moves = 0;
	game.winner = 'C';
}

bool	BotTicTacToe::fillField(game_t &game, int spot, char put)
{
	if (spot < 0 || spot > 8)
		throw(std::range_error("Invalid spot " + std::to_string(spot + 1)));
	if (game.field[spot / 3][spot % 3] != ' ')
		throw(std::range_error("Spot " + std::to_string(spot + 1)+ " is already taken"));
	if (put != 'X' && put != 'O')
		throw(std::range_error("Trying to fill invalid character " + std::to_string(put)));

	game.field[spot / 3][spot % 3] = put;
	int add;
	if (put == 'X')
		add = 1;
	else
		add = -1;
	game.field[spot / 3][3] += add;
	game.field[3][spot % 3] += add;
	if (spot / 3 == spot % 3)
		game.diag[0] += add;
	if (spot / 3 + spot % 3 == 2)
		game.diag[1] += add;
	++game.moves;
	return (true);
}

void	BotTicTacToe::updateGame(std::string key, game_t &update)
{
	this->game[key] = update;
}

void	BotTicTacToe::sendGame(std::string dest)
{
	game_t	game = this->findGame(dest);
	this->sendGame(dest, game);
}

void	BotTicTacToe::sendGame(std::string dest, game_t &game)
{
	this->gameOver(game);

	this->send.push("PRIVMSG " + dest + " : " + game.field[0][0] + " | " + game.field[0][1] + " | " + game.field[0][2]);
	this->send.push("PRIVMSG " + dest + " : " + game.field[1][0] + " | " + game.field[1][1] + " | " + game.field[1][2]);
	this->send.push("PRIVMSG " + dest + " : " + game.field[2][0] + " | " + game.field[2][1] + " | " + game.field[2][2]);
	if (game.winner != 'C')
	{
		if (game.winner == 'X')
		{
			this->send.push("PRIVMSG " + dest + " :You won! (Difficulty " + std::to_string(game.level) + ')');
			if (game.level < 5)
			{
				++game.level;
				this->send.push("PRIVMSG " + dest + " :Setting level to " + std::to_string(game.level));
			}
		}
		else if (game.winner == 'O')
		{
			this->send.push("PRIVMSG " + dest + " :I won! (Difficulty " + std::to_string(game.level) + ')');
			if (game.level > 1)
			{
				--game.level;
				this->send.push("PRIVMSG " + dest + " :Setting level to " + std::to_string(game.level));
			}
		}
		else if (game.winner == 'T')
			this->send.push("PRIVMSG " + dest + " :We tied! (Difficulty " + std::to_string(game.level) + ')');
		this->clearGame(game);
	}
}

bool	BotTicTacToe::stillActive(void) const
{
	return (true);
}

void	BotTicTacToe::sendMsg(std::string msg)
{
	std::string	name(msg.substr(1, msg.find('!') - 1));
	std::string args(msg.substr(msg.find(' ') + 1));
	std::tuple<AClient &, std::string, std::vector<std::string> >\
				prsd(Parse::parseMsg(*this, args));

	this->botRespond(name, std::get<1>(prsd), std::get<2>(prsd));
}

std::string	BotTicTacToe::getMsg(void)
{
	std::string	reply;

	if (!this->send.empty())
	{
		reply = this->send.front() + "\r\n";
		this->send.pop();
	}
	return (reply);
}

void	BotTicTacToe::closeFD(void)
{
	while (!this->send.empty())
		this->send.pop();
}

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */
