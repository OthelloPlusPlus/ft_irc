/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ohengelm <ohengelm@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/11 13:37:13 by ohengelm      #+#    #+#                 */
/*   Updated: 2023/03/11 13:39:19 by ohengelm      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Client.hpp"

#include <netinet/in.h>
// struct sockaddr_in
// socklen_t
#include <poll.h>
// struct pollfd
#include <vector>
// std::vector

class Server
{
	private:
		struct sockaddr_in	socketAddress;
		socklen_t			socketAddressLen;
		struct pollfd		pollInfo;

		std::vector<Client *> clients;

		void	setSocket(void);
		void	acceptClient(void);

	protected:

	public:
		Server(void);
		Server(const Server &src);
		~Server(void);

		bool	initialize(void);
		bool	checkNewClients(void);
		bool	checkClients(void);

		Server	&operator=(const Server &src);
};

#endif
