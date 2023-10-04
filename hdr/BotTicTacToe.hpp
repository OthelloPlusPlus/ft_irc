#ifndef BOTTICTACTOE_HPP
# define BOTTICTACTOE_HPP

class Server;
# include "AClient.hpp"

# include <queue>

class BotTicTacToe: public AClient
{
	private:
		// int	pipeFD[2];
		std::queue<std::string>	msgs;

		// bool		readReceive(void);
		// std::string	botRespond(std::string msg);
		std::string	botRespond(AClient &src, const std::string cmd, const std::vector<std::string> &args);
		std::string	botRespondInvite(const std::vector<std::string> &args);
		std::string	botRespondPart(const std::vector<std::string> &args);

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
