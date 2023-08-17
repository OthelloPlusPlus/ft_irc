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

		std::vector<std::string> cmd;
		std::string				_buffer;
		std::string				username;
		std::string				nickname;
		// bool					admin;

	protected:

	public:
		Client(int serverFD);
		Client(const Client &src);
		~Client(void);

		void		initialize(int serverFd);
		void		sendMsg(std::string msg);
		std::string	getMsg(void);
		std::string	getBuff(void);
		void		setBuff(std::string buffer);
		bool		stillActive(void) const;
		// void	printInfo(void) const;

		Client	&operator=(const Client &src);
};

#endif

