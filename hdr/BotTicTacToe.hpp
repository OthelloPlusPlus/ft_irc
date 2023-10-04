#ifndef BOTTICTACTOE_HPP
# define BOTTICTACTOE_HPP

class Server;
# include "AClient.hpp"

# include <queue>

class BotTicTacToe: public AClient
{
	private:
		// int	pipeFD[2];
		std::queue<std::string>	recv;
		std::queue<std::string> send;

		// bool		readReceive(void);
		// std::string	botRespond(std::string msg);
		void	botRespond(std::string name, AClient &src, const std::string cmd, const std::vector<std::string> &args);
		void	botRespondInvite(const std::vector<std::string> &args);
		void	botRespondPart(const std::vector<std::string> &args);
		void	botRespondPrivMsg(std::string name, const std::vector<std::string> &args);
		void	think(std::string dest, const std::vector<std::string> &args);

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
