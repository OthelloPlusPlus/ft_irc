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

# ifndef CLIENT_HPP
#  include "Client.hpp"
# else
class Client;
# endif
# ifndef CHANNEL_HPP
#  include "Channel.hpp"
# else
class Channel;
# endif

#include <string>
// std::string
# include <poll.h>
// struct pollfd
# include <netinet/in.h>
// struct sockaddr_in
# include <vector>
// std::vector

class Server
{
	private:
		std::string				serverName;
		struct pollfd			pollInfo;
		struct sockaddr_in		socketAddress;
		int						port;
		std::string				publicIP;
		std::string				localIP;

		std::vector<Channel *>	channels;
		std::vector<Client *>	clients;
		std::string				passwordUser;

		// std::string	getHostIp(void);
		void	setLocalIP(void);
		void	bootUpServer(void);
		void	readEnv(void);

		void	acceptClient(void);
		void	sendWelcome(Client *client);

	protected:

	public:
		// Server(void);
		Server(int argc, char **argv);
		Server(const Server &src);
		~Server(void);

		void	checkNewClient(void);
		void	checkClients(void);
		void	checkChannels(void);

		int	validatePassword(const std::string password) const;
		// bool	nicknameExists(const std::string nickname) const;

		std::vector<Client *>	getClientList(void);
		Client					*getClient(std::string name) const;
		Channel					*getChannel(std::string channel) const;
		std::string				getName(void) const;
		const std::string		getIP(void) const;

		void	joinChannel(Client *client, const std::string channelName);
		void	partChannel(Client *client, const std::string channelName);
		void	sendChannelList(Client *client) const;
		void	sendWho(Client *client, const std::string who) const;
		void	sendWhoIs(Client *client, const std::string who) const;
		void	sendPong(Client *client) const;
		void	sendPong(Client *client, const std::string token) const;
		void	sendPrivMsg(const Client *client, const std::vector<std::string> &args);
		void	sendInvite(Client *client, const std::vector<std::string> &args);
		void	setChannelTopic(Client &client, const std::vector<std::string> &args);
		void	setChannelMode(Client &client, const std::vector<std::string> &args);

		Server	&operator=(const Server &src);
};

#endif
