/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 19:24:50 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/06 19:23:37 by emlicame      ########   odam.nl         */
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
#include <string>
// std::string
#include <vector>
// std::vector
#include <tuple>   
// for std::tuple

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
		
		bool				_password;

		Client(const Client &src);
		Client	&operator=(const Client &src);

		void	initialize(int serverFd);
		bool	readReceive(void);
		bool	pollConnection(void);
		bool	hasPassword(void) const;
		void	setPollInfofd(int val);


	protected:	

	public:
		Client(Server &server);
		~Client(void);

		std::vector<FileTransfer> fileTransfers;

		std::string	getMsg(void);
		void		sendMsg(std::string msg);
		void		printInfo(void) const;
		
		std::string	getBestName (void) const; // remove - use getnickName
		std::string	getSourceName(void) const;

		void		passwordValidation(bool val);
		void		setIsRegistered(bool val);


		bool		stillActive(void) const;
		void		closeFD(void);
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
		// void	setIsOperator(bool val);