#ifndef ACLIENT_HPP
# define ACLIENT_HPP

# include <string>
// std::string

class AClient
{
	private:

	protected:
		std::string	_serverName;
		std::string	_IpHostName;

		std::string	_nickName;
		std::string	_userName; //Ident Name
		std::string	_realName;
		bool		_isRegistered;
		bool		_isOperator;
	
		std::string	_buffer;

		int			_temp;

	public:
		AClient(std::string serverName);
		virtual ~AClient(void) = 0;

		virtual std::string	getMsg(void) = 0;
		virtual void		sendMsg(std::string msg) = 0;
		virtual bool		stillActive(void) const = 0;

		void	setServerName(std::string serverName);
		void	setIpHostName(std::string IpHostName);
		void	setNickName(std::string nickName);
		void	setUserName(std::string userName);
		void	setRealName(std::string realName);
		void	setIsRegistered(bool value);
		void	setIsOperator(bool value);
		void	setBuffer(std::string buffer);

		const std::string	&getServerName(void) const;
		const std::string	&getIpHostName(void) const;
		const std::string	&getNickName(void) const;
		const std::string	&getUserName(void) const;
		const std::string	&getRealName(void) const;
		const bool			&getIsRegistered(void) const;
		const bool			&getIsOperator(void) const;
		const std::string	&getBuffer(void) const;

		std::string	getSourceName(void) const
			{return ("AOthello");}
		std::string	const &getServer(void) const
			{return (this->_serverName);}
		int const	&getPollInfofd(void) const
			{return (this->_temp);}
		void	setPollInfofd(int val)
			{this->_temp = val;}
		
};

#endif