#ifndef SERVERBOT_HPP
# define SERVERBOT_HPP

# include "AClient.hpp"

# include <string>

class ServerBot: public AClient
{
	private:

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
