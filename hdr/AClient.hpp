#ifndef ACLIENT_HPP
# define ACLIENT_HPP

class Server;

# include <string>
// std::string

class AClient
{
	private:
		bool		_isOperator;

	protected:
		Server		&_server;

		std::string	_clientIP;
		std::string	_nickName;
		std::string	_userName; //Ident Name
		std::string	_realName;
		bool		_isRegistered;
	
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
		const std::string	&getBestName(void) const;
		const bool			&getIsRegistered(void) const;
		const bool			&getIsOperator(void) const;
		const std::string	&getBuffer(void) const;

		bool	operator==(const AClient &cmp) const;
		bool	operator!=(const AClient &cmp) const;
		bool	operator>(const AClient &cmp) const;
		bool	operator>=(const AClient &cmp) const;
		bool	operator<(const AClient &cmp) const;
		bool	operator<=(const AClient &cmp) const;
		template <typename T>
		friend std::string	operator+(const T add, const AClient &src)
		{
			static_assert(std::is_same<T, char>::value || 
							std::is_same<T, char*>::value || 
							std::is_same<T, const char*>::value ||
							std::is_same<T, std::string>::value, "Invalid type");
			std::string	ret;

			ret = add;
			return (ret + src._nickName + "!~" + src._userName + '@' + src._clientIP);
		}
		template <typename T>
		std::string operator+(const T add)
		{
			static_assert(std::is_same<T, char>::value || 
							std::is_same<T, char*>::value || 
							std::is_same<T, const char*>::value ||
							std::is_same<T, std::string>::value, "Invalid type");
			std::string ret;

			ret = add;
			return (ret + this->_nickName + "!~" + this->_userName + '@' + this->_clientIP);
		}
};

#include "Server.hpp"

#else

class AClient;

#endif
