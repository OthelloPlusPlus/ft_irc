#ifndef ACLIENT_HPP
# define ACLIENT_HPP

class Server;

# include <string>
// std::string

class AClient
{
	private:

	protected:
		Server		&_serverAdd;
		// std::string	&_serverName;
		std::string	_IpHostName;

		std::string	_nickName;
		std::string	_userName; //Ident Name
		std::string	_realName;
		bool		_isRegistered;
		bool		_isOperator;
	
		std::string	_buffer;

		int			_temp;

	public:
		AClient(Server &server);
		virtual ~AClient(void) = 0;

		virtual std::string	getMsg(void) = 0;
		virtual void		sendMsg(std::string msg) = 0;
		virtual bool		stillActive(void) const = 0;

		// void	setServerName(std::string serverName);
		void	setIpHostName(std::string IpHostName);
		void	setNickName(std::string nickName);
		void	setUserName(std::string userName);
		void	setRealName(std::string realName);
		void	setIsRegistered(bool value);
		void	setIsOperator(bool value);
		void	setBuffer(std::string buffer);

		// const std::string	&getServerName(void) const;
		const Server		*getServerAddr(void) const;
		const std::string	&getIpHostName(void) const;
		const std::string	&getNickName(void) const;
		const std::string	&getUserName(void) const;
		const std::string	&getRealName(void) const;
		const bool			&getIsRegistered(void) const;
		const bool			&getIsOperator(void) const;
		const std::string	&getBuffer(void) const;

		virtual std::string	getSourceName(void) const
			{return ("AOthello");}
		virtual std::string	const &getServer(void) const
			{return (this->_IpHostName);}
		virtual int const	&getPollInfofd(void) const
			{return (this->_temp);}
		virtual void	setPollInfofd(int val)
			{this->_temp = val;}
		
};

#include "Server.hpp"

#else

class AClient;

#endif
