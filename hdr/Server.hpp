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

# include "Client.hpp"

# include <iostream>
// std::
# include <poll.h>
// struct pollfd
# include <netinet/in.h>
// struct sockaddr_in
# include <vector>
// std::vector

class Server
{
	private:
		struct pollfd			pollInfo;
		struct sockaddr_in		socketAddress;
		std::vector<Client *>	clients;
		int			port;
		std::string	password;
		std::string	ip;

		std::string	getHostIp(void) const;
		void		bootUpServer(void);
		void		acceptClient(void);

	protected:

	public:
		// Server(void);
		Server(int argc, char **argv);
		Server(const Server &src);
		~Server(void);

		void	checkNewClient(void);
		void	checkClients(void) const;

		bool	validatePassword(const std::string password) const;

		Server	&operator=(const Server &src);
};

#endif
