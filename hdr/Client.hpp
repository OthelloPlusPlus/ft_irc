/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Client.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: emlicame <emlicame@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/18 19:24:50 by emlicame      #+#    #+#                 */
/*   Updated: 2023/10/10 19:18:51 by emlicame      ########   odam.nl         */
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
# include <map>
// std::map



// typedef struct file_s
// {
// 	std::string		fileName;
// 	std::string		senderName;
// 	std::string		receiverName;
// 	std::string 	line;
// }	file_t;

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

		// std::map<std::string, file_t>	fileTr;

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
