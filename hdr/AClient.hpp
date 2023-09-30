#ifndef ACLIENT_HPP
# define ACLIENT_HPP

class Server;

# include <string>
// std::string

class AClient
{
	private:

	protected:
		Server		&_server;

		std::string	_clientIP;
		std::string	_nickName;
		std::string	_userName; //Ident Name
		std::string	_realName;
		bool		_isRegistered;
		bool		_isOperator;
	
		std::string	_buffer;

	public:
		AClient(Server &server);
		virtual ~AClient(void) = 0;

		virtual bool		stillActive(void) const = 0;
		virtual void		closeFD(void) = 0;
		virtual std::string	getMsg(void) = 0;
		virtual void		sendMsg(std::string msg) = 0;

		virtual void		passwordValidation(bool val);
		virtual void		setIsRegistered(bool value);
		virtual void		printInfo(void) const;
		virtual std::string	getSourceName(void) const;

		void	setClientIP(std::string clientIP);
		void	setNickName(std::string nickName);
		void	setUserName(std::string userName);
		void	setRealName(std::string realName);
		void	setIsOperator(bool value);
		void	setBuffer(std::string buffer);
		Server				*getServer(void) const;
		const std::string	&getClientIP(void) const;
		const std::string	&getNickName(void) const;
		const std::string	&getUserName(void) const;
		const std::string	&getRealName(void) const;
		const bool			&getIsRegistered(void) const;
		const bool			&getIsOperator(void) const;
		const std::string	&getBuffer(void) const;
};

#include "Server.hpp"

#else

class AClient;

#endif
