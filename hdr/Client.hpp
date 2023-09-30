/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 19:24:50 by emlicame      #+#    #+#                 */
/*   Updated: 2023/09/30 16:13:59 by emlicame      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "AClient.hpp"

#include <netinet/in.h>
// struct sockaddr_in
// socklen_t
#include <poll.h>
// struct pollfd
#include <iostream>
// std::
#include <vector>

struct FileTransfer
{
	std::string		targetName;
	std::string		fileName;
	std::string		hostTarget;
	u_int32_t		port;
};

class Client : public AClient
{
	private:
		struct sockaddr_in	socketAddress;
		socklen_t			socketAddressLen;
		struct pollfd		pollInfo;
		
		// std::string			_server;
		bool				_hasPassword;

		Client(const Client &src);
		Client	&operator=(const Client &src);

		void	initialize(int serverFd);
		bool	readReceive(void);
		bool	pollConnection(void);


	protected:	

	public:
		Client(Server &server);
		~Client(void);

		std::vector<FileTransfer> fileTransfers;
		
		// std::string	const &getServer(void) const; 		// server IP
		// std::string	const &getIpHostName(void) const; 	// client IP
		int const	&getPollInfofd(void) const; 
		bool		hasPassword(void) const;
		std::string	getBestName (void) const;
		std::string	getSourceName(void) const;

		
		void	userRegistration(void);
		// void	setServer(std::string server);
		void	setPollInfofd(int val);
		void	setHasPassword(bool val);
		bool	stillActive(void) const;

		std::string	getMsg(void);
		void	sendMsg(std::string msg);
		void	userNotRegisteredMsg(std::string cmd);
		void	userNotOperatorMsg(std::string cmd);
		void	printInfo(void) const;

		void	closeFD(void);
};

#else

class Client;

#endif

		// std::string			_nickName;
		// std::string			_userName; //Ident Name
		// std::string			_realName;
		// bool				_isRegistered;
		// bool				_isOperator;
		// std::string			_buffer;
		// std::string			_message;
		
		// std::string	const &getMessage(void) const;
		// std::string	const &getUserName(void) const; 
		// std::string	const &getRealName(void) const;
		// std::string	const &getNickName(void) const;
		// bool		getIsRegistered(void) const;
		// bool		getIsOperator(void) const;
		
		// void	setMessage(std::string message);
		// void	setNickName(std::string nickname);
		// void	setUserName(std::string username);
		// void	setRealName(std::string realname);
		// void	setIsRegistered(bool val);
		// void	setIsOperator(bool val);