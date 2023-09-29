#ifndef BOTTICTACTOE_HPP
# define BOTTICTACTOE_HPP

# include "AClient.hpp"

class BotTicTacToe: public AClient
{
	private:
		int	pipeFD[2];

		bool	readReceive(void);
		void	botRespond(std::string msg);
		
	protected:

	public:
		BotTicTacToe(std::string serverName);
		~BotTicTacToe(void);

		std::string	getMsg(void);
		void		sendMsg(std::string msg);
		bool		stillActive(void) const;
};

#endif