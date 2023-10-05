#ifndef BOTTICTACTOE_HPP
# define BOTTICTACTOE_HPP

class Server;
# include "AClient.hpp"

# include <queue>
# include <map>
typedef struct game_s
{
	int		level;
	char	field[3][3];
	char	winner;
}	game_t;

class BotTicTacToe: public AClient
{
	private:
		// int	pipeFD[2];
		std::queue<std::string>	recv;
		std::queue<std::string> send;
		std::map<std::string, game_t>	game;

		// bool		readReceive(void);
		// std::string	botRespond(std::string msg);
		void	botRespond(std::string name, AClient &src, const std::string cmd, const std::vector<std::string> &args);
		void	botRespondInvite(const std::vector<std::string> &args);
		void	botRespondPart(const std::vector<std::string> &args);
		void	botRespondPrivMsg(std::string name, const std::vector<std::string> &args);
		void	think(std::string dest, const std::vector<std::string> &args);
		bool	enterMove(std::string dest, std::string arg);
		void	counterMove(std::string key);
		int		counterMoveWin(game_t &game, char player);
		int		counterMoveSmart(game_t &game);
		int		counterMoveRandom(game_t &game);
		int		countMoves(std::string key);
		int		countMoves(game_t &game);
		game_t	findGame(std::string key);
		void	newGame(std::string key);
		void	clearGame(std::string key);
		void	updateGame(std::string key, game_t update);
		void	sendGame(std::string dest);
		bool	gameOver(std::string dest);
		void	setLevel(std::string arg, std::string dest);

	protected:

	public:
		BotTicTacToe(Server &server);
		~BotTicTacToe(void);

		std::string	getMsg(void);
		void		sendMsg(std::string msg);
		bool		stillActive(void) const;
		void		closeFD(void);
};

# include "Server.hpp"

#else

class BotTicTacToe;

#endif
