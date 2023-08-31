/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/11 13:37:13 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/11 13:39:19 by ohengelm      ########   odam.nl         */
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
		std::string			_userName; //Identity Name???
		std::string			_password;
		std::string			_identName;
		std::string			_realName;
		std::string			_IpHostName;
		std::string			_server;
		bool				_isRegistered;
		bool				_hasPassword;
		// bool				admin;

		std::vector<std::string> _cmd;
		std::string				_buffer;
		std::string				_message;
	protected:

	public:
		Client(int serverFD);
		Client(const Client &src);
		~Client(void);

		void		initialize(int serverFd);
		bool		stillActive(void) const;

		void		sendMsg(std::string msg) const;
		std::string	getMsg(void);
		bool		readReceive(int sockfd);

		std::string	const &getBuff(void) const;
		std::string const &getUserName(void) const; 
		std::string const &getIdentName(void) const;
		std::string const &getRealName(void) const;
		std::string const &getNickName(void) const;
		std::string const &getPassword(void) const;
		std::string const &getServer(void) const; 		// computer of server
		std::string const &getIpHostName(void) const; 	// computer of client
		int const &getPollInfofd(void) const; 
		bool getIsRegistered( void ) const;
		bool hasPassword( void ) const;

		void setBuff(std::string buffer);
		void setUserName(std::string username);
		void setIdentName(std::string identname);
		void setRealName(std::string realname);
		void setNickName(std::string nickname);
		void setPassword(std::string password);
		void setServer(std::string server);
		void setPollInfofd(int val);
		void setIpHostName(std::string ipAddress);
		void setIsRegistered(bool val);
		void setHasPassword(bool val);
		void userRegistration( void );

		Client	&operator=(const Client &src);
		void	printInfo(void) const;
};

#endif

/*


Othello 1
Othello 2
Othello 3
Othello 4
Othello 5
Othello 6
Othello 7
Othello 8
Othello 9
Othello 10
Othello 11
Othello 12
Othello 13
Othello 14
Othello 15

*/