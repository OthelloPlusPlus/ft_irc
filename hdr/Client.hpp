/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emlicame <emlicame@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 19:24:50 by emlicame          #+#    #+#             */
/*   Updated: 2023/09/18 19:24:53 by emlicame         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <netinet/in.h>
// struct sockaddr_in
// socklen_t
#include <poll.h>
// struct pollfd
#include <iostream>
// std::
#include <vector>

class Client
{
	private:
		struct sockaddr_in	socketAddress;
		socklen_t			socketAddressLen;
		struct pollfd		pollInfo;

		std::string			_nickName;
		std::string			_userName; //Ident Name
		std::string			_identName;
		std::string			_realName;
		std::string			_IpHostName;
		std::string			_server;
		bool				_isRegistered;
		bool				_isOperator;
		bool				_hasPassword;
		std::string			_buffer;
		std::string			_message;

		static int				_verbose;
		Client(const Client &src);
		Client	&operator=(const Client &src);

		void		initialize(int serverFd);
		bool		readReceive(int sockfd);


	protected:

	public:
		Client(int serverFD);
		~Client( void );
		std::string	const &getBuff( void ) const;
		std::string const &getUserName( void ) const; 
		std::string const &getIdentName( void ) const;
		std::string const &getRealName( void ) const;
		std::string const &getNickName( void ) const;
		std::string const &getServer( void ) const; 		// server IP
		std::string const &getIpHostName( void ) const; 	// client IP
		int const	&getPollInfofd( void ) const; 
		bool 		getIsRegistered( void ) const;
		bool 		getIsOperator( void ) const;
		bool		hasPassword( void ) const;
		std::string getBestName ( void ) const;

		void setNickName(std::string nickname);
		void setBuff(std::string buffer);
		void userRegistration( void );
		void setUserName(std::string username);
		void setIdentName(std::string identname);
		void setRealName(std::string realname);
		void setServer(std::string server);
		void setPollInfofd(int val);
		void setIpHostName(std::string ipAddress);
		void setIsRegistered(bool val);
		void setIsOperator(bool val);
		void setHasPassword(bool val);
		static void	setVerbose(const int verbose);
		bool stillActive( void ) const;

		std::string	getMsg( void );
		void		sendMsg(std::string msg) const;
		void		printInfo( void ) const;
};

#endif
