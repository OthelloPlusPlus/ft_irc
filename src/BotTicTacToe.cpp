/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   BotTicTacToe.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/09 20:45:12 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "BotTicTacToe.hpp"
#include "Command.hpp"
#include "Parse.hpp"
#include "verboseCheck.hpp"
#include "colors.hpp"

#include <iostream>
// std::
#include <unistd.h>
// int pipe(int pipefd[2]);
// int close(int fd);
#include <strings.h>
// void bzero(void *s, size_t n);
#include <string.h>
// char *strerror(int errnum);
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
	this->_isOperator = false;
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	// if (pipe(this->pipeFD) == -1)
	// 	throw(std::runtime_error("pipe(): "));
	std::cout	<< C_DGREEN	<< "Default constructor "
				<< C_GREEN	<< "BotTicTacToe"
				<< C_DGREEN	<< " called."
				<< C_RESET	<< std::endl;
}

// BotTicTacToe::BotTicTacToe(const BotTicTacToe &src)
// {
// 	*this = src;
// 	std::cout	<< C_DGREEN	<< "Copy constructor "
// 				<< C_GREEN	<< "BotTicTacToe"
// 				<< C_DGREEN	<< " called."
// 				<< C_RESET	<< std::endl;
// }

/** ************************************************************************ **\
 * 
 * 	Deconstructors
 * 
\* ************************************************************************** */

BotTicTacToe::~BotTicTacToe(void)
{
	// if (this->pipeFD[0] > 2)
	// 	close(this->pipeFD[0]);
	// if (this->pipeFD[1] > 2)
	// 	close(this->pipeFD[1]);
	this->closeFD();
	std::cout	<< C_DRED	<< "Deconstructor "
				<< C_RED	<< "BotTicTacToe"
				<< C_DRED	<< " called"
				<< C_RESET	<< std::endl;
}

/** ************************************************************************ **\
 * 
 * 	Member Functions
 * 
\* ************************************************************************** */

bool	BotTicTacToe::stillActive(void) const
{
	return (true);
}

std::string	BotTicTacToe::getMsg(void)
{
	std::string	reply;

	if (!this->recv.empty())
	{
		std::string	name = this->recv.front().substr(1, this->recv.front().find('!') - 1);
		std::string	args = this->recv.front().substr(this->recv.front().find(' ') + 1);

		std::tuple<AClient &, std::string, std::vector<std::string>> prsd = Parse::parseMsg(*this, args);

		this->botRespond(name, std::get<0>(prsd), std::get<1>(prsd), std::get<2>(prsd));
		this->recv.pop();
	}
	if (!send.empty())
	{
		reply = this->send.front();
		this->send.pop();
	}
	return (reply);
}

void	BotTicTacToe::botRespond(std::string name, AClient &src, const std::string cmd, const std::vector<std::string> &args)
{
	if (cmd == "INVITE")
		this->botRespondInvite(args);
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
	this->send.push("JOIN " + channel->getName() + "\r\n");
	this->send.push("PRIVMSG " + channel->getName() + " :Anyone want to //play a game?\r\n");
}

void	BotTicTacToe::botRespondPart(const std::vector<std::string> &args)
{
	Channel *channel = this->_server.getChannel(args[0]);

	if (channel != nullptr && channel->getSize() <= 1)
		this->send.push("PART " + channel->getName() + "\r\n");
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
			dest = client->getBestName();
	}
	if (!dest.empty())
		this->think(dest, args);
}

void	BotTicTacToe::think(std::string dest, const std::vector<std::string> &args)
{
	std::string	cmd = args[1].substr(0, args[1].find(' '));

	if (cmd == "//play")
	{
		try
		{
			game_t	game = this->findGame(dest);

			if (this->enterMove(game, args[1]))
				this->counterMove(game);
			this->sendGame(dest, game);
			this->updateGame(dest, game);
		}
		catch(const std::exception& e)
		{
			this->send.push("PRIVMSG " + dest + " :" + e.what() + "\r\n");
		}
		
	}
	else if (cmd == "//show")
	{
		this->setLevel("", dest);
		this->sendGame(dest);
	}
	else if (cmd == "//level")
		this->setLevel(args[1], dest);
	else if (cmd == "//help")
		this->send.push("PRIVMSG " + dest + " :I can respond to //play and //level\r\n");
	else if (dest[0] != '#')
		this->send.push("PRIVMSG " + dest + " :Huh?\r\n");
}

bool	BotTicTacToe::enterMove(game_t &game, std::string arg)
{
	size_t	pos1 = arg.find(' ');
	size_t	pos2 = arg.find(' ', pos1 + 1);
	int		playerPos;

	if (pos1 == pos2)
	{
		if (game.field[3][3] == 0)
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

void	BotTicTacToe::counterMove(game_t &game)
{
	int		spot = 0;

	if (this->gameOver(game))
		return ;
	if (game.level >= 2 && spot == 0)
	{
		spot = this->counterMoveWin(game, -1);
		std::cout	<< "counterMoveWin()\t"	<< spot	<< std::endl;
	}
	if (game.level >= 3 && spot == 0)
	{
		spot = this->counterMoveWin(game, 1);
		std::cout	<< "counterMoveLose()\t"	<< spot	<< std::endl;
	}
	if (game.level >= 4 && spot == 0)
	{
		spot = this->counterMoveOpen(game);
		std::cout	<< "counterMoveOpen()\t"	<< spot	<< std::endl;
	}
	if (game.level >= 4 && spot == 0)
	{
		spot = this->counterMoveSmart(game);
		std::cout	<< "counterMoveSmart()\t"	<< spot	<< std::endl;
	}
	if (spot == 0)
	{
		spot = this->counterMoveRandom(game);
		std::cout	<< "counterMoveRandom()\t"	<< spot	<< std::endl;
	}
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
	int	spot[3][3];
	int	move = 0;
	int	high = -7;

	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
			if (game.field[x][y] == ' ' && game.field[x][3] + game.field[3][y] > high)
			{
				high = game.field[x][3] + game.field[3][y];
				move = x * 3 + y + 1;
			}
	return (move);
}

int	BotTicTacToe::counterMoveRandom(game_t &game)
{
	int	moves = rand() % (9 - game.moves);
	for (int x = 0; x < 3; ++x)
		for (int y = 0; y < 3; ++y)
		{
			if (game.field[x][y] == ' ')
			{
				if (moves == 0)
					return (x * 3 + y + 1);
				else
					--moves;
			}
		}
	return (0);
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

	newGame.level = 4;
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

void	BotTicTacToe::updateGame(std::string key, game_t &update)
{
	this->game[key] = update;
}

void	BotTicTacToe::sendGame(std::string dest, game_t &game)
{
	this->gameOver(game);

	this->send.push("PRIVMSG " + dest + " : " + game.field[0][0] + " | " + game.field[0][1] + " | " + game.field[0][2] + "\r\n");
	this->send.push("PRIVMSG " + dest + " : " + game.field[1][0] + " | " + game.field[1][1] + " | " + game.field[1][2] + "\r\n");
	this->send.push("PRIVMSG " + dest + " : " + game.field[2][0] + " | " + game.field[2][1] + " | " + game.field[2][2] + "\r\n");
	if (game.winner != 'C')
	{
		if (game.winner == 'X')
			this->send.push("PRIVMSG " + dest + " :You won!\r\n");
		else if (game.winner == 'O')
			this->send.push("PRIVMSG " + dest + " :I won!\r\n");
		else if (game.winner == 'T')
			this->send.push("PRIVMSG " + dest + " :We tied!\r\n");
		this->clearGame(game);
	}
}

void	BotTicTacToe::sendGame(std::string dest)
{
	game_t	game = this->findGame(dest);
	this->sendGame(dest, game);
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

void	BotTicTacToe::setLevel(std::string arg, std::string dest)
{
	size_t	pos1 = arg.find(' ');
	size_t	pos2 = arg.find(' ', pos1 + 1);
	game_t	game = findGame(dest);

	if (pos1 != pos2)
	{
		try
		{
			int	level = std::stoi(arg.substr(pos1 + 1, pos2));
			if (level >= 0 && level <= 4)
				game.level = level;
			else
				throw (std::range_error(""));
			this->updateGame(dest, game);
		}
		catch(const std::exception& e)
		{
			this->send.push("PRIVMSG " + dest + " :Can't set level to " + arg.substr(pos1 + 1, pos2) + "\r\n");
			return ;
		}
	}
	this->send.push("PRIVMSG " + dest + " :Level: " + std::to_string(game.level) + "\r\n");
}

void	BotTicTacToe::sendMsg(std::string msg)
{
	this->recv.push(msg);
	// msg = ':' + this->_server.getName() + ' ' + msg + "\r\n";

	// ssize_t	size = send(this->pipeFD[0], msg.c_str(), msg.length(), 0);
	// if (verboseCheck() >= V_MSG)
	// 	std::cout	<< C_RESET	<< "Send ["	<< size	<< "]\t"
	// 				<< C_LORANGE	<< msg
	// 				<< C_RESET	<< std::flush;
}

void	BotTicTacToe::closeFD(void)
{
	while (!this->recv.empty())
		this->recv.pop();
	while (!this->send.empty())
		this->send.pop();
	// if (this->pipeFD[0] > 2)
	// {
	// 	close(this->pipeFD[0]);
	// 	this->pipeFD[0] = -1;
	// }
	// if (this->pipeFD[1] > 2)
	// {
	// 	close(this->pipeFD[1]);
	// 	this->pipeFD[1] = -1;
	// }
}

/** ************************************************************************ **\
 * 
 * 	Operators
 * 
\* ************************************************************************** */

// BotTicTacToe	&BotTicTacToe::operator=(const BotTicTacToe &src)
// {
// 	if (this == &src)
// 		return (*this);

// 	return (*this);
// }
