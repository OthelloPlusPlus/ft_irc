#ifndef SERVERBOT_HPP
# define SERVERBOT_HPP

# include "AClient.hpp"

# include <string>
// std::string
# include <queue>
// std::queue

class ServerBot: public AClient
{
	private:
		std::queue<std::string>	send;
		//Bot Responces
		void	botRespond(std::string name, AClient &src, const std::string cmd, const std::vector<std::string> &args);
		void	botRespondPrivMsg(std::string name, const std::vector<std::string> &args);
		void	think(std::string dest, std::string arg);
		void	thinkHelp(std::string dest);
		void	thinkShutdown(std::string dest, std::string arg);

	protected:

	public:
		//(De)constructors
		ServerBot(Server &server);
		~ServerBot(void);
		//Virtual void functions from AClient
		bool		stillActive(void) const;
		void		sendMsg(std::string msg);
		std::string	getMsg(void);
		void		closeFD(void);
};

#else

class ServerBot;

#endif
