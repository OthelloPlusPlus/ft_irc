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
# include "Channel.hpp"

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
		std::vector<Channel *>	channels;
		int			port;
		std::string	password;
		std::string	localIP;
		std::string	publicIP;

		// std::string	getHostIp(void);
		void		setLocalIP(void);
		void		bootUpServer(void);
		void		acceptClient(void);
		void		joinChannel(Client *client, const std::string channelName);
		void		sendWelcome(Client *client);

	protected:

	public:
		// Server(void);
		Server(int argc, char **argv);
		Server(const Server &src);
		~Server(void);

		void	checkNewClient(void);
		void	checkClients(void);

		bool	validatePassword(const std::string password) const;
		// bool	nicknameExists(const std::string nickname) const;

		std::vector<Client *>	getClientList(void);
		Client	*getClient(std::string name) const;

		void	sendChannelList(const Client *client) const;
		void	sendWhoIs(const Client *client, const std::string who) const;
		void	sendPong(const Client *client) const;

		Server	&operator=(const Server &src);
};

#endif
