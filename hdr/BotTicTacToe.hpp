/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   BotTicTacToe.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/03 20:34:08 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/09 20:45:12 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTTICTACTOE_HPP
# define BOTTICTACTOE_HPP

# include "AClient.hpp"

# include <string>
// std::string
# include <queue>
// std::queue<>
# include <map>
// std::map<>

typedef struct game_s
{
	int		level;
	char	field[4][4];
	char	diag[2];
	int		moves;
	char	winner;
}	game_t;

class BotTicTacToe: public AClient
{
	private:
		std::queue<std::string> 		send;
		std::map<std::string, game_t>	game;

		//Bot Responces
		void	botRespond(std::string name, AClient &src, const std::string cmd, const std::vector<std::string> &args);
		void	botRespondInvite(const std::vector<std::string> &args);
		void	botRespondJoin(std::string name, const std::vector<std::string> &args);
		void	botRespondPart(const std::vector<std::string> &args);
		void	botRespondPrivMsg(std::string name, const std::vector<std::string> &args);
		//Bot Responces for game commands
		void	think(std::string dest, const std::string arg);
		void	thinkLevel(std::string dest, std::string arg);
		void	thinkShow(std::string dest);
		void	thinkReset(std::string dest);
		void	thinkHelp(std::string dest);
		//Bot Algorithms for playing TicTacToe
		void	thinkPlay(std::string dest, std::string arg);
		bool	enterMove(game_t &game, std::string arg);
		void	counterMove(game_t &game);
		int		counterMoveWin(game_t &game, int player);
		int		counterMoveOpen(game_t &game);
		int		counterMoveSmart(game_t &game);
		int		counterMoveRandom(game_t &game);
		bool	gameOver(game_t &game);
		//Bot Game settings and transfer
		game_t	findGame(std::string key);
		void	newGame(std::string key);
		void	clearGame(game_t &game);
		bool	fillField(game_t &game, int spot, char put);
		void	updateGame(std::string key, game_t &update);
		void	sendGame(std::string dest);
		void	sendGame(std::string dest, game_t &game);

	protected:

	public:
		//(De)constructors
		BotTicTacToe(Server &server);
		~BotTicTacToe(void);
		//Virtual void functions from AClient
		bool		stillActive(void) const;
		void		sendMsg(std::string msg);
		std::string	getMsg(void);
		void		closeFD(void);
};

#else

class BotTicTacToe;

#endif
