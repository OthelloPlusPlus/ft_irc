#ifndef ROCKPAPERSCISSORS_HPP
#define ROCKPAPERSCISSORS_HPP

class Server;

# include "AClient.hpp"

# include <queue>
# include <map>

typedef struct hand_s
{
	int level;
	int moves[2];
}	hand_t;

class RockBot: public AClient
{
	private:
		std::queue<std::string>	recv;
		std::queue<std::string> send;
		std::map<std::string, hand_t>	hand;

		void	rockBotRespond(std::string name, AClient &src, const std::string cmd, const std::vector<std::string> &args);
		void	botRespondInvite(const std::vector<std::string> &args);
		void	botRespondJoin(const std::vector<std::string> &args);


	public:
		RockBot(Server &server);
		~RockBot(void);

		std::string	getMsg(void);
		void		sendMsg(std::string msg);
		bool		stillActive(void) const;
		void		closeFD(void);

};

# include "Server.hpp"

#else

class RockBot;

#endif