#ifndef ACLIENT_HPP
# define ACLIENT_HPP

# include <string>
// std::string

class AClient
{
	private:

	protected:
		std::string	_serverName;
		std::string	_nickName;
		std::string	_userName; //Ident Name
		std::string	_realName;
		bool		_isRegsitered;
		bool		_isOperator;
		std::string	_buffer;

	public:
		AClient(std::string serverName);
		virtual ~AClient(void);

		virtual std::string	getMsg(void) = 0;
		virtual void		sendMsg(std::string msg) = 0;
		virtual bool		stillActive(void) const = 0;

		void	setServerName(std::string serverName);
		void	setNickName(std::string nickName);
		void	setUserName(std::string userName);
		void	setRealName(std::string realName);
		void	setIsRegistered(bool value);
		void	setIsOperator(bool value);
		void	setBuffer(std::string buffer);

		const std::string	&getServerName(void) const;
		const std::string	&getNickName(void) const;
		const std::string	&getUserName(void) const;
		const std::string	&getRealName(void) const;
		const bool			&getIsRegistered(void) const;
		const bool			&getIsOperator(void) const;
		const std::string	&getBuffer(void) const;
};

#endif