#ifndef BOTTICTACTOE_HPP
# define BOTTICTACTOE_HPP

class Server;
# include "AClient.hpp"

class BotTicTacToe: public AClient
{
	private:
		int	pipeFD[2];

		bool		readReceive(void);
		std::string	botRespond(std::string msg);
		
	protected:

	public:
		BotTicTacToe(Server &server);
		~BotTicTacToe(void);

		std::string	getMsg(void);
		void		sendMsg(std::string msg);
		bool		stillActive(void) const;
};

# include "Server.hpp"

#else

class BotTicTacToe;

#endif
